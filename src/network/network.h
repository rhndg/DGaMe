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
#include "../main/globals.h"
#include "../game_map/game_map.h"

using namespace std;

class Network {
private:
	
	enum packType {
		syncPack,gamePack,controlPack,syncReqPack,gameReqPack
	};
//control and data session should be distinct
	int movNum,numPlayers,dataSession,controlSession,playerId,packSize,packCount,mvsLen	//number of moves(keytaps)
		,maxTime,minTime,sendGameTo,sockNum;												//session id (never 0)
																		//number of players
																		//id of this player
				 														//size of packet
																		//number of redundant packets
																		//length allotted to moves in synchro data
																		//
	game_map* dGame

	map< int,vector <sockaddr_in> > address;					//socket addressed of given id
	map< int,bool > isPeer;										//current connected peers
	map< int,vector<game_map::key_tap> > syncData;				//recieved data
	map< pair<int,int>,vector<char> > gameData;					//recieved data for recon to be sent data for rec data
	map< int,vector <int> > syncReqs;						//requested data of players by the given id
	map< int,vector <int,int> > gameReqs;					//requested data of players by the given id

	vector<pair<int,vector<char> > > syncBuf;					//contains sync data
	vector<vector<char> > gameBuf;
	int delay;													//time delay for message to reach
	

	vector<char> bufInit(packType type,int packId,int packCount);	//push the first 8 bytes acc to pack type

	void encodeSync(vector<game_map::key_tap> moves);
	void encodeSyncReqs();
	void encodeGameReqs();
	void encodeGame(int forId,game_map* aGame);						//encodes game state		
	void decodeSync(vector<char> pack);								//analyzes recieved packets and updates state
	void decodeGame(vector<char> pack,game_map* aGame);				//updates agame with data
	void decodeSyncReq(vector<char> pack);
	void decodeGameReq();

	void sendSyncBuf();												//send contents of the buffer
	void sendGameBuf();
	void recBuf();													//loads all data from socket to buffer wih int field -1
	void wait();													//wait for time delay
	void incCounter();												//increaments counters
	void clearTemp();												//clears temp data for restarting process
public:
	Network();
	Network(string host);
	~Network();
	void* data_thread(void* x);
	void* control_thread(void* x);
	//vector of non human players, data recieved ***empty if disconnected
};
#endif