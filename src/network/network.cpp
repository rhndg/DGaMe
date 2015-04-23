#include "network.h"


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
	movNum=10;
	numPlayers=1;
	dataSession=-1;
	controlSession=-1;
	playerId=rand();
	packSize=128;
	packExtra=3;
	mvsLen=10;
	maxTime=100000;
	minTime=50000;
	sendGameTo=-1;
	delay=100;
	iniSampleTime=100000;
	dcCount=100;
	startSync=false;
	isDisconnected=false;
	
	pthread_mutex_init(&mvsmutx,0);
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

void Network::encodeSync(vector<game_map::key_tap> moves){
	vector<char> buffer=bufInit(syncPack,0,3);
	moves.resize(mvsLen,game_map::nop);
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
				
				vector<game_map::key_tap> moves = syncData[reqIds[i]];
				if(moves.size()!=0){
					moves.resize(mvsLen,game_map::nop);
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
			syncData[playerID].push_back(((game_map::key_tap)x));
			syncData[playerID].push_back(((game_map::key_tap)y));
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
					decodeSync(buffer);
					break;
				case syncReqPack :
					decodeSyncReq(buffer);
					break;
				case controlPack :
					//add ips etc etc
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
	if(diff<=5&&diff>=0) return true;
	else return false;
}

void Network::recBuf(){
	vector<char> buffer ;
	buffer.resize(packSize,-1);
	while(buffer[0]!=0){
		bzero(&buffer[0],packSize);
		recvfrom(fd,&buffer[0],packSize,0,NULL,0);
		int session=((int)((buffer[1]+256)%256))*256+((int)((buffer[0]+256)%256));
		if (futureSession(session)&&buffer[0]!=0) syncBuf[session].push_back(pair<int,vector<char> >(-1,buffer));
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
	if(syncData.empty())  isDisconnected=true;
	else{
		for(int i=0;i!=numPlayers;i++){
			if(isPeer[i]&&syncData[i].empty()){
				isPeer[i]=false;
				disconnected[i]=true;
			}
		}
	}
}

bool Network::isDc(){
	for(int i=0;i!=syncData.size();i++){
		if(isPeer[i]&&syncData[i].empty()) return true;
	}
	return false;
}

vector<char> Network::encodeIps(){
	vector<char> buffer=bufInit(newIp,65000,65000);
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
	vector<char> temp,buffer=bufInit(newIp,65000,65000);
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
	playerId = pack[8];
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

void* Network::data_thread(void* x){
	vector<char> buffer;
	buffer.resize(packSize,0);
	socklen_t len;
	struct sockaddr_in addr;
	if(x!=NULL){
		sockaddr_in* host=((sockaddr_in*)x);
		buffer=encodeJoinGame();
		for(int i=0;i!=packExtra;i++)sendto(fd,&buffer[0],buffer.size(),0,(struct sockaddr*)host,sizeof(*host));
	
	}else{
		playerId=1;
		numPlayers=1;
	}
	
	cout<<ntohs(mySock.sin_port)<<endl;

	while(!startSync){
		
		// prnt (buffer);
			memset(((char*)&addr),0,sizeof(addr));
			buffer.clear();
			buffer.resize(packSize,0);
			recvfrom(fd,&buffer[0],packSize,0,(struct sockaddr*)&addr,&len);
			
			if(buffer[0]!=0&&buffer[2]!=playerId&&addr.sin_port!=0&&addr.sin_addr.s_addr!=0){
			// cout<<ntohs(mySock.sin_port)<<endl;

			// prnt(buffer);
				switch ((packType)buffer[3]){
					case joinGame :
					{
						if(!joinedGame[pair<unsigned long,unsigned short>(addr.sin_addr.s_addr,addr.sin_port)]){
													
							unsigned long ip=addr.sin_addr.s_addr;
							unsigned short port=addr.sin_port;
							
							encodeNewIp(ip,port);
							syncBuf[dataSession].push_back(pair<int,vector<char> >(numPlayers+1,encodeIps()));
							
							address[numPlayers+1]=addr;
							isPeer[numPlayers+1]=true;
							numPlayers++;
							
							sendSyncBuf();
							joinedGame[pair<unsigned long,unsigned short>(addr.sin_addr.s_addr,addr.sin_port)]=true;
							// cout<<1<<endl;
						}
					}
					break;
					case newIp :
					if(numPlayers==1||(!isPeer[((int)buffer[9])]&&((int)buffer[9])!=-1)){
						if(!isPeer[((int)buffer[2])]){
							address[((int)buffer[2])]=addr;
							isPeer[((int)buffer[2])]=true;
							numPlayers++;
						}
						// prnt (buffer);
						decodeNewIp(buffer);
					}
					break;
					case startGame :
					startSync=true;
					break;
					default:
					break;
				}
			}
		
		// cout<<1<<endl;
		usleep(iniSampleTime);
	}
	
	syncBuf[dataSession].clear();

	while(startSync){
		
		clearTemp();
		pthread_mutex_lock(&mvsmutx);
		syncData[playerId]=moves;
		encodeSync(moves);
		moves.clear();
		pthread_mutex_unlock(&mvsmutx);
		sendSyncBuf();
		usleep(delay);
		recBuf();
		decodeSyncBuf();
		encodeSyncReqs();
		sendSyncBuf();
		usleep(delay);
		recBuf();
		decodeSyncBuf();

		int count=0;
		while(isDc()&&count<dcCount){
			usleep(delay/2);
			recBuf();
			decodeSyncBuf();
			count++;
		}

		pthread_barrier_wait(&b1);
		resolveDc();
		incCounter();
		pthread_barrier_wait(&b2);
		//barrier
	}
}
void* Network::event_thread(void*x){
	while(1){
		pthread_barrier_wait(&b1);
		pthread_barrier_wait(&b2);	
	}
}

void Network::start(){
	// startSync=true;
	// vector<char> buffer=bufInit(startGame,65000,65000);
	// buffer.resize(packSize,((char)-1));
	// for(int i =0;i<=numPlayers;i++){
	// 	if(isPeer[i]){
	// 		syncBuf.push_back(pair<int,vector<char> >(i,buffer));
	// 	}
	// }
	// sendSyncBuf();
}


int main(int argc,char** argv) {
    Network X;
    
    struct sockaddr_in host;
    if(argc==3){
	    host.sin_family=AF_INET;
	    host.sin_addr.s_addr=inet_addr(argv[1]);
		host.sin_port=htons(atoi(argv[2]));
		X.data_thread(((void*)&host));
    }else{
    	X.data_thread(NULL);
    }

    // struct sockaddr_in si_me, si_other;
    // int s, i, blen, slen = sizeof(si_other);
    // char buf[BUFLEN];

    // s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    // if (s == -1)
    //     die("socket");

    // memset((char *) &si_me, 0, sizeof(si_me));
    // si_me.sin_family = AF_INET;
    // si_me.sin_port = htons(1234);
    // si_me.sin_addr.s_addr = htonl(192.168.1.1);

    // if (bind(s, (struct sockaddr*) &si_me, sizeof(si_me))==-1)
    //     die("bind");

    // int blen = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*) &si_other, &slen);
    // if (blen == -1)
    //    diep("recvfrom()");

    // printf("Data: %.*s \nReceived from %s:%d\n\n", blen, buf, inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

    // //send answer back to the client
    // if (sendto(s, buf, blen, 0, (struct sockaddr*) &si_other, slen) == -1)
    //     diep("sendto()");

    // close(s);

    return 0;

}