#include "network.h"

Network::Network(){

}
Network::Network(string host){

}
void Network::clearTemp(){
	syncData.clear();
	gameData.clear();
	syncReqs.clear();
	gameReqs.clear();
	syncBuf.clear();
	gameBuf.clear();
	packId=0;
}
vector<char> Network::bufInit(packType t,int packId,int packCount){
	vector<char> buffer;
	int x;
	if (t!=controlPack)
		x=dataSession%256;
	else
		x=controlSession%256;
	buffer.push_back(((char)x);
	if (t!=controlPack)
		x=dataSession/256;
	else
		x=controlSession/256;
	buffer.push_back(((char)x);
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
	while(moves.size()<mvsLen) moves.push_back(nop);
	buffer.push_back(((char)playerId));
	for(int i = 0;i<mvsLen;i+=2){
		int x=(int)moves.back();
		moves.pop_back();
		x=x*16+((int)moves.back());
		buffer.push_back(((char)x));
		moves.pop_back();
	}
	buffer.resize(packSize,((char)-1));
	for(int i=1;i<=numPlayers;i++){
		if (isPeer[i]) syncBuf.push_back(pair<int,vector<char> >(i,buffer));
	}
}

void Network::encodeSyncAns(){
	vector<char> temp=bufInit(syncPack,1,3),buffer;
	for(int j=1;j<=numPlayers;j++){
		if(isPeer[j]){
			reqIds=syncReqs[j];
			for(int i = 0;i!=reqIds.size();i++){
				buffer=temp;
				vector<game_map::key_tap> moves = syncData[reqIds[i]];
				if(moves.size()!=0){
					moves.resize(mvsLen,nop);
					buffer.push_back(((char)reqIds[i]));
					while(!moves.empty()){
						int x=(int)moves.back();
						moves.pop_back();
						x=x*16+((int)moves.back());
						buffer.push_back(((char)x));
						moves.pop_back();
					}
				}
			}
		buffer.resize(packSize,((char)-1));
		syncBuf.push_back(pair<int,vector<char> >(j,buffer));
		}
	}
}

void Network::encodeSyncReqs(){
	vector<int> residue;
	for(int i=0; i<= numPlayers;i++){
		if(isPeer[i]&&syncData[i].empty()){
			residue.push_back(i);
		}
	}
	vector<char> buffer = bufInit(syncReqPack,2,3);
	for(int i=0;i!=residue.size();i++) buffer.push_back(((char)residue[i]));
	residue.resize(packSize,((char)-1));
	syncBuf.push_back(buffer);
}

void Network::decodeSync(vector<char> data){
	int i=8;playerID;
	while(data[i]!=-1&&syncData[data[i]].empty()){
		playerID=data[i]
		i++;
		int x,y,j=0;
		while(j!=mvsLen/2){
			x=data[i+j]/16;
			y=data[i+j]%16;
			syncData[playerID].push_back(((game_map::key_tap)x));
			syncData[playerID].push_back(((game_map::key_tap)y));
			j++;
		}
		i+=j;
	}
}

void Network::decodeSyncReq(vector<char> pack){
	int fromPlayer=pack[2];
	vector<int> temp;
	for(int i=8;pack[i]!=-1;i++){
		syncReqs[fromPlayer].push_back(((int)pack[i]));
	}
}
//playerid 2 , packid 4,5
void decodeSyncBuf(){
	map<pair<int,int>,bool>hist;
	vector<char> buffer;
	while(!syncBuf.empty()){
		buffer=syncBuf.back();
		syncBuf.pop_back();
		int sender=((int)(buffer[2]+256)%256),
		packID=((int)((buffer[4]+256)%256))*256+((int)((buffer[5]+256)%256));
		if(!hist[pair<int,int>(sender,packID)]){
			switch ((packType)buffer[3]){
				case: syncPack
					decodeSync(buffer);
					break;
				case: syncReqPack
					decodeSyncReq(buffer);
					break;
				case: controlPack
					//add ips etc etc
					break;
			}
			hist[pair<int,int>(sender,packID)]=true;
		}
	}
}

int main(void) {
    struct sockaddr_in si_me, si_other;
    int s, i, blen, slen = sizeof(si_other);
    char buf[BUFLEN];

    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == -1)
        die("socket");

    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(1234);
    si_me.sin_addr.s_addr = htonl(192.168.1.1);

    if (bind(s, (struct sockaddr*) &si_me, sizeof(si_me))==-1)
        die("bind");

    int blen = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*) &si_other, &slen);
    if (blen == -1)
       diep("recvfrom()");

    printf("Data: %.*s \nReceived from %s:%d\n\n", blen, buf, inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

    //send answer back to the client
    if (sendto(s, buf, blen, 0, (struct sockaddr*) &si_other, slen) == -1)
        diep("sendto()");

    close(s);
    return 0;
}