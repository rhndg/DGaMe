#include "network.h"

Network X;
vector<int> XXX;

Network::Network(){
	srand(time(NULL));
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	fcntl(fd, F_SETFL, O_NONBLOCK);
	memset(((char*)&mySock),0,sizeof(mySock));
	mySock.sin_family=AF_INET;
	mySock.sin_addr.s_addr = htonl(INADDR_ANY); 
	mySock.sin_port = htons(0);
	bind(fd, (struct sockaddr *)&mySock, sizeof(mySock));
	socklen_t len = sizeof (mySock);
	getsockname(fd,(struct sockaddr *)&mySock, &len);
	moves.resize(mvsLen,game_map_::nop);
	movNum=10;
	numPlayers=1;
	numBots=0;
	dataSession=-1;
	controlSession=-1;
	playerId=rand();
	packSize=128;
	packExtra=3;
	mvsLen=10;
	maxTime=100000;
	minTime=50000;
	sendGameTo=-1;
	delay=70000;
	iniSampleTime=1000;
	dcCount=100;
	startSync=false;
	isDisconnected=false;
	isHost=false;
	firstFrame=true;
	frameCount=1;
	syncPeriod=7;
	randSeed=0;
	pthread_mutex_init(&strt, NULL);
	pthread_mutex_init(&bufMutx, NULL);
	pthread_barrier_init(&b1, 0, 2);
	pthread_barrier_init(&b2, 0, 2);
}
Network::~Network(){
}

void Network::clearTemp(){
	syncData.clear();
	gameData.clear();
	syncReqs.clear();
	gameReqs.clear();
	gameBuf.clear();
	syncHist.clear();
	gameHist.clear();
}
void Network::incCounter(){
	syncBuf[dataSession].clear();
	dataSession=(dataSession+1)%65536;
	if(dataSession%256==0) dataSession=dataSession+1%65536;
}

vector<char> Network::bufInit(Network::packType t,int packId,int packCount){
	vector<char> buffer;
	int x;
	if (t!=controlPack)
		x=dataSession%256;
	else
		x=controlSession%256;
	buffer.push_back(((char)x));
	if (t!=controlPack)
		x=dataSession/256;
	else
		x=controlSession/256;
	buffer.push_back(((char)x));
	buffer.push_back(((char)playerId));
	buffer.push_back(((char)t));
	x=packId/256;
	buffer.push_back(x);
	x=packId%256;
	buffer.push_back(x);
	x=packCount/256;
	buffer.push_back(x);
	x=packCount%256;
	buffer.push_back(x);
	return buffer;
}

void Network::encodeSync(vector<game_map_::key_tap> moves){
	vector<char> buffer=bufInit(syncPack,0,3);
	moves.resize(mvsLen,game_map_::nop);
	buffer.push_back(((char)playerId));
	for(int i = 0;i<mvsLen;i+=2){
		int x=(int)moves[i];
		x=x*16+((int)moves[i+1]);
		buffer.push_back(((char)x));
	}
	buffer.resize(packSize,((char)-1));
	for(int i=1;i<=numPlayers;i++){
		if (isPeer[i]) syncBuf[dataSession].push_back(pair<int,vector<char> >(i,buffer));
	}
}

void Network::encodeSyncAns(){
	vector<char> temp=bufInit(syncPack,1,3),buffer;
	for(int j=1;j<=numPlayers;j++){
		if(isPeer[j]){
			vector<int>reqIds=syncReqs[j];
			buffer=temp;
			for(int i = 0;i!=reqIds.size();i++){
				
				vector<game_map_::key_tap> moves = syncData[reqIds[i]];
				if(moves.size()!=0){
					moves.resize(mvsLen,game_map_::nop);
					buffer.push_back(((char)reqIds[i]));
					for(int i = 0;i<mvsLen;i+=2){
						int x=(int)moves[i];
						x=x*16+((int)moves[i+1]);
						buffer.push_back(((char)x));
					}
				}
			}
			buffer.resize(packSize,((char)-1));
			syncBuf[dataSession].push_back(pair<int,vector<char> >(j,buffer));
		}
	}
}

void Network::encodeSyncReqs(){
	vector<char> buffer = bufInit(syncReqPack,2,3);
	for(int i=0; i<= numPlayers;i++){
		if(isPeer[i]&&syncData[i].empty()){
			buffer.push_back(((char)i));
		}
	}
	buffer.resize(packSize,((char)-1));
	for(int i=0;i<=numPlayers;i++){
		if(isPeer[i]){
			syncBuf[dataSession].push_back(pair<int,vector<char> >(i,buffer));
		}
	}
}

void Network::decodeSync(vector<char> data){
	int i=8,playerID;
	while(data[i]!=-1&&syncData[data[i]].empty()){
		playerID=data[i];
		i++;
		int x,y,j=0,k;
		while(j!=mvsLen/2){
			k=(data[i+j]+256)%256;
			x=k/16;
			y=k%16;
			syncData[playerID].push_back(((game_map_::key_tap)x));
			syncData[playerID].push_back(((game_map_::key_tap)y));
			j++;
		}
		i+=j;
	}
}

void Network::decodeSyncReq(vector<char> pack){
	int fromPlayer=(int)pack[2];
	for(int i=8;pack[i]!=-1;i++){
		syncReqs[fromPlayer].push_back(((int)pack[i]));
	}
}
//playerid 2 , packid 4,5
void Network::decodeSyncBuf(){
	
	vector<char> buffer;
	while(!syncBuf[dataSession].empty()){
		buffer=syncBuf[dataSession].back().second;
		syncBuf[dataSession].pop_back();
		int sender=((int)(buffer[2]+256)%256),
		packID=((int)((buffer[4]+256)%256))*256+((int)((buffer[5]+256)%256));
		if(!syncHist[pair<int,int>(sender,packID)]){
			
			switch ((packType)buffer[3]){
				case syncPack :
					// prnt (buffer);
					decodeSync(buffer);
					break;
				case syncReqPack :
					decodeSyncReq(buffer);
					break;
				default :

					break;
			}
			syncHist[pair<int,int>(sender,packID)]=true;
		}
	}
}

bool Network::futureSession(int packSess){
	int diff = (packSess-dataSession+65536)%65536;
	if(diff<=100&&diff>=0) return true;
	else return false;
}

void Network::recBuf(){
	vector<char> buffer ;
	buffer.resize(packSize,((char)-1));
	while(buffer[0]!=0){
		bzero(&buffer[0],packSize);
		recvfrom(fd,&buffer[0],packSize,0,NULL,0);
		// prnt (buffer);
		int session=((int)((buffer[1]+256)%256))*256+((int)((buffer[0]+256)%256));
		if (futureSession(session)&&buffer[0]!=0) {
			// prnt (buffer);
			syncBuf[session].push_back(pair<int,vector<char> >(-1,buffer));
		}
	}
}

void Network::sendSyncBuf(){
	for(int i=0;i!=syncBuf[dataSession].size();i++){
		int len = sizeof(address[syncBuf[dataSession][i].first]);
		for(int j=0;j!=packExtra;j++){
			// cout<<ntohs(address[syncBuf[i].first].sin_port)<<endl;
			sendto(fd, &syncBuf[dataSession][i].second[0] , packSize, 0, (struct sockaddr*) &address[syncBuf[dataSession][i].first], len);
		}
	}
	syncBuf[dataSession].clear();
}

void Network::resolveDc(){
	if(syncData.size()==1)  isDisconnected=true;
	else{
		for(int i=0;i<=numPlayers;i++){
			if(isPeer[i]&&syncData[i].empty()){
				isPeer[i]=false;
				disconnected[i]=true;
			}
		}
	}
}

bool Network::isDc(){
	for(int i=0;i<=numPlayers;i++){
		if(isPeer[i]&&syncData[i].empty()) return true;
	}
	return false;
}

vector<char> Network::encodeIps(){
	vector<char> buffer=bufInit(newIp,randSeed,numBots);
	int ind=9;
	buffer.resize(packSize,((char)-1));
	buffer[8]=((char)numPlayers+1);
	for (int i=0;i<=numPlayers;i++){
		if(isPeer[i]){
			buffer[ind]=((char)i);
			ind++;
			unsigned long ip = address[i].sin_addr.s_addr;
			memcpy(&buffer[ind],((char*)&ip),4);
			ind+=4;
			unsigned short port=address[i].sin_port;
			memcpy(&buffer[ind],((char*)&port),2);
			ind+=2;
		}
	}
	return buffer;
}


void Network::encodeNewIp(unsigned long Ip,unsigned short port){
	vector<char> temp,buffer=bufInit(newIp,randSeed,numBots);
	buffer.resize(packSize,((char)-1));
	buffer[9]=((char)numPlayers+1);
	memcpy(&buffer[10],((char*)&Ip),4);
	memcpy(&buffer[14],((char*)&port),2);
	for(int i=0;i<=numPlayers;i++){
		if(isPeer[i]){
			buffer[8]=((char)i);
			syncBuf[dataSession].push_back(pair<int,vector<char> >(i,buffer));
		}
	}
}

void Network::decodeNewIp(vector<char> pack){
	numBots=pack[7];
	playerId = pack[8];
	randSeed=(int)((pack[4]+256)%256)*256+((pack[5]+256)%256);
	int ind=9,pId;
	while(pack[ind]!=-1){
		pId=pack[ind];
		ind++;
		if(!isPeer[pId]){
			unsigned short port;
			unsigned long Ip;
			memcpy(((char*)&Ip),&pack[ind],4);
			ind+=4;
			memcpy(((char*)&port),&pack[ind],2);
			ind+=2;
			address[pId].sin_family=AF_INET;
			address[pId].sin_port=port;
			address[pId].sin_addr.s_addr=Ip;
			numPlayers++;
			isPeer[pId]=true;
		}else{
			ind+=6;
		}
	}
}

vector<char> Network::encodeJoinGame(){
	vector<char> buffer=bufInit(joinGame,6500,6500);
	buffer.resize(packSize,((char)-1));
	return buffer;
}


void event_thread(){
	if(!X.startSync){	
		
//*****************************************
//display rasmmmmmm	
		usleep(X.delay);
//**********************************************

	}else{
		if(X.frameCount==0){
			pthread_barrier_wait(&X.b1);
			if(X.firstFrame){
				X.firstFrame=false;
			}
			else{
				
				//update game state using syncData*****************
				cout<<"**************************"<<endl;
				for(int i=1;i<=X.numPlayers;i++){
					cout<<i<<"==>";
					for(int j=0;j!=X.syncData[i].size();j++){
						cout<<((int)X.syncData[i][j])<<",";
					}
					cout<<endl;
				}
				cout<<"**************************"<<endl;
				//*************************************************
			}
			pthread_mutex_lock(&X.bufMutx);
			reverse(XXX.begin(),XXX.end());
			XXX.resize(X.mvsLen,((int)game_map_::nop));		
			reverse(XXX.begin(),XXX.end());
			X.moves.clear();
			for(int i=0;i!=XXX.size();i++) X.moves.push_back((game_map_::key_tap)XXX[i]);
			XXX.clear();
			pthread_mutex_unlock(&X.bufMutx);
			pthread_barrier_wait(&X.b2);
		}

		X.frameCount=(X.frameCount+1)%X.syncPeriod;
		
//********************************************************************************
		XXX.push_back(((game_map_::key_tap)(rand()%10)));
		usleep(X.delay);
		//display game

//****************************************************************************
	}

	event_thread();
}


void* data_thread(void* x){
	
	X.print_addresses(AF_INET);
	cout<<"port => "<<ntohs(X.mySock.sin_port)<<endl;

	vector<char> buffer;
	buffer.resize(X.packSize,0);
	socklen_t len;
	struct sockaddr_in addr;
	if(x!=NULL){
		sockaddr_in* host=((sockaddr_in*)x);
		buffer=X.encodeJoinGame();
		for(int i=0;i!=X.packExtra;i++)sendto(X.fd,&buffer[0],buffer.size(),0,(struct sockaddr*)host,sizeof(*host));
	
	}else{
		X.playerId=1;
		X.numPlayers=1;
	}
	
	

	while(!X.startSync){
		
		pthread_mutex_lock (&X.strt);


			memset(((char*)&addr),0,sizeof(addr));
			buffer.clear();
			buffer.resize(X.packSize,0);
			recvfrom(X.fd,&buffer[0],X.packSize,0,(struct sockaddr*)&addr,&len);
			
			if(buffer[0]!=0&&buffer[2]!=X.playerId&&addr.sin_port!=0&&addr.sin_addr.s_addr!=0){
				switch ((Network::packType)buffer[3]){
					case Network::joinGame :
					{
						if(!X.joinedGame[pair<unsigned long,unsigned short>(addr.sin_addr.s_addr,addr.sin_port)]){
													
							unsigned long ip=addr.sin_addr.s_addr;
							unsigned short port=addr.sin_port;
							
							X.encodeNewIp(ip,port);
							X.syncBuf[X.dataSession].push_back(pair<int,vector<char> >(X.numPlayers+1,X.encodeIps()));
							
							X.address[X.numPlayers+1]=addr;
							X.isPeer[X.numPlayers+1]=true;
							X.numPlayers++;
							
							X.sendSyncBuf();
							X.joinedGame[pair<unsigned long,unsigned short>(addr.sin_addr.s_addr,addr.sin_port)]=true;
							// cout<<1<<endl;
						}
					}
					break;
					case Network::newIp :
					if(X.numPlayers==1||(!X.isPeer[((int)buffer[9])]&&((int)buffer[9])!=-1)){
						if(!X.isPeer[((int)buffer[2])]){
							X.address[((int)buffer[2])]=addr;
							X.isPeer[((int)buffer[2])]=true;
							X.numPlayers++;
						}
						// prnt (buffer);
						X.decodeNewIp(buffer);
					}
					break;
					case Network::startGame :
					X.startSync=true;

					break;
					default:
					break;
				}
			}
		pthread_mutex_unlock (&X.strt);
		usleep(X.iniSampleTime);
	}
	X.sendSyncBuf();
	X.dataSession=1;
	srand(X.randSeed);

//********************************************************

cout<<"numPlayers - "<<X.numPlayers<<endl;
cout<<"numBots - "<<X.numBots<<endl;
cout<<"rand seed - "<<X.randSeed<<endl;

//********************************************************
	
	while(!X.isDisconnected){
		
		
		X.clearTemp();
		X.syncData[X.playerId]=X.moves;
		X.encodeSync(X.syncData[X.playerId]);
		X.sendSyncBuf();
		usleep(X.delay);
		X.recBuf();
		X.decodeSyncBuf();
		X.encodeSyncReqs();
		X.sendSyncBuf();
		usleep(X.delay);
		X.recBuf();
		X.decodeSyncBuf();
		X.encodeSyncAns();
		X.sendSyncBuf();
		usleep(X.delay);
		X.recBuf();
		X.decodeSyncBuf();
		int count=0;
		while(X.isDc()&&count<X.dcCount){
			usleep(X.delay/2);
			X.recBuf();
			X.decodeSyncBuf();
			count++;
		}

		pthread_barrier_wait(&X.b1);
		X.resolveDc();
		X.incCounter();
		pthread_barrier_wait(&X.b2);
		//barrier
	}
}


void Network::start(){
	if(isHost){
		pthread_mutex_lock (&strt);
		startSync=true;
		vector<char> buffer=bufInit(startGame,65000,65000);
		buffer.resize(packSize,((char)-1));
		for(int i =1;i<=numPlayers;i++){
			if(isPeer[i]){
				syncBuf[dataSession].push_back(pair<int,vector<char> >(i,buffer));
			}
		}
//**********************************************

// gameInit(numPlayers,numBots,playerId);

//***********************************************		
		usleep(iniSampleTime);
		pthread_mutex_unlock (&strt);
	}
}

int Network::print_addresses(const int domain){
  int s;
  struct ifconf ifconf;
  struct ifreq ifr[50];
  int ifs;
  int i;

  s = socket(domain, SOCK_STREAM, 0);
  if (s < 0) {
    perror("socket");
    return 0;
  }

  ifconf.ifc_buf = (char *) ifr;
  ifconf.ifc_len = sizeof ifr;

  if (ioctl(s, SIOCGIFCONF, &ifconf) == -1) {
    perror("ioctl");
    return 0;
  }

  ifs = ifconf.ifc_len / sizeof(ifr[0]);
  printf("interfaces = %d:\n", ifs);
  for (i = 0; i < ifs; i++) {
    char ip[INET_ADDRSTRLEN];
    struct sockaddr_in *s_in = (struct sockaddr_in *) &ifr[i].ifr_addr;

    if (!inet_ntop(domain, &s_in->sin_addr, ip, sizeof(ip))) {
      perror("inet_ntop");
      return 0;
    }

    printf("%s - %s\n", ifr[i].ifr_name, ip);
  }

  close(s);

  return 1;
}

//***********************************

void* dc(void*){
	int i;
	cin>>i;
	X.start();
}

//***********************************

int main(int argc,char** argv) {
    
    
    struct sockaddr_in host;
    if(argc==3){
	    host.sin_family=AF_INET;
	    host.sin_addr.s_addr=inet_addr(argv[1]);
		host.sin_port=htons(atoi(argv[2]));
		pthread_create(&X.data,NULL,data_thread,((void*)&host));
    }else{
    	if(argc==2)X.numBots=atoi(argv[1]);
    	X.isHost=true;
		X.randSeed=(rand())%65536;
    	pthread_create(&X.data,NULL,data_thread,NULL);
    }

//**********************************************************************

pthread_t kk;
pthread_create(&kk,NULL,dc,NULL);

event_thread();

pthread_mutex_lock(&X.bufMutx);
pthread_mutex_unlock(&X.bufMutx);
// on click call start
// use mutex and pushback into xxx int cast

//GLUT MAIN LOOP   


//***********************************************************************

}