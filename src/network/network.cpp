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
		x=dataSession/256;
	else
		x=controlSession/256;
	buffer.push_back(((char)x);
	if (t!=controlPack)
		x=dataSession%256;
	else
		x=controlSession%256;
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
	vector<char> buffer=bufInit(syncPack,0,1);
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

void Network::encodeSyncReqs(){
	vector<char> temp=bufInit(syncPack,0,1),buffer;
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

