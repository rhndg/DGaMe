#ifndef NETWORK_H
#define NETWORK_H

#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include <fcntl.h>
#include <pthread.h>
// #include "../main/globals.h"
// #include "../game_map/game_map.h"

using namespace std;
class game_map{
public:
	enum key_tap{
		up,down,left,rght,nop
	};
};

class Network {
private:
	
	enum packType {
		syncPack,gamePack,controlPack,syncReqPack,gameReqPack,joinGame,newIp,startGame
	};
	bool startSync,isHost,isDisconnected;

//control and data session should be distinct
	int movNum,numPlayers,dataSession,controlSession,playerId,packSize,packExtra,mvsLen	//number of moves(keytaps)
		,maxTime,minTime,sendGameTo,sockNum,delay,fd,iniSampleTime,dcCount;						//session id (never 0)
																		//number of players
																		//id of this player
				 														//size of packet
																		//number of redundant packets
																		//length allotted to moves in synchro data
																		//
	pthread_mutex_t mvsmutx;
	pthread_barrier_t b1,b2;

	game_map* dGame;
	sockaddr_in mySock;
	map< int,sockaddr_in> address;								//socket addressed of given id
	map< pair<unsigned long,unsigned short>,bool>joinedGame;
	map< int,bool > isPeer,disconnected;										//current connected peers
	map< int,vector<game_map::key_tap> > syncData;				//recieved data
	map< pair<int,int>,vector<char> > gameData;					//recieved data for recon to be sent data for rec data
	map< int,vector <int> > syncReqs;							//requested data of players by the given id
	vector<int> gameReqs;						                //requested data of players by the given id
	map<pair<int,int>,bool>syncHist;
	map<int,bool>gameHist;
	map<int,vector<pair<int,vector<char> > > > syncBuf;					//contains sync data
	vector<vector<char> > gameBuf;
	vector<game_map::key_tap> moves;
		
	

	vector<char> bufInit(packType type,int packId,int packCount);	//push the first 8 bytes acc to pack type

	void encodeSync(vector<game_map::key_tap> moves);
	void encodeSyncReqs();
	void encodeGameReqs();
	void encodeGame(int forId);										//encodes game state		
	void decodeSync(vector<char> pack);								//analyzes recieved packets and updates state
	void decodeGame(vector<char> pack,game_map* aGame);				//updates agame with data
	void decodeSyncReq(vector<char> pack);
	void decodeGameReq();
	void decodeNewIp(vector<char> pack);

	vector<char> encodeIps();
	vector<char> encodeJoinGame();
	void encodeNewIp(unsigned long Ip,unsigned short port);
	void sendSyncBuf();												//send contents of the buffer
	void decodeSyncBuf();											//can contain sync data ,sync req, control
	void decodeGameBuf();											//can contain game data ,game req
	void sendGameBuf();
	void recBuf();														//session no checked here loads all data from socket to buffer wih int field -1 
	void wait();													//wait for time delay
	void resolveDc();
	void incCounter();												//increaments counters
	void clearTemp();												//clears temp data for restarting process
	void encodeSyncAns();
	void start();
	bool isDc();
	bool futureSession(int packSess);
	//decode**************************
	void prnt (vector<char> v){
		for(int i=0;i!=v.size();i++) cout<<((int)v[i])<<" ";
		cout<<endl;
	}
	void prnt_(vector<game_map::key_tap>v){
		for(int i=0;i!=v.size();i++) cout<<((int)v[i])<<" ";
		cout<<endl;

	}
	//********************************


public:
	Network();
	~Network();
	pthread_t data,event;
	void* data_thread(void* x);
	void* event_thread(void* x);
	//vector of non human players, data recieved ***empty if disconnected
};
#endif