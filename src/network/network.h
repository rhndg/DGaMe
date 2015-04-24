#ifndef NETWORK_H
#define NETWORK_H

#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stropts.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/netdevice.h>
#include <netdb.h>
#include <arpa/inet.h>
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
public:
	
/**
 * @brief packet types to be sent
 */	
	enum packType {
		syncPack,gamePack,controlPack,syncReqPack,gameReqPack,joinGame,newIp,startGame
	};
	bool startSync,isHost,isDisconnected;

//control and data session should be distinct
	int movNum,numPlayers,dataSession,controlSession,playerId,packSize,packExtra,mvsLen	//number of moves(keytaps)
		,maxTime,minTime,sendGameTo,sockNum,delay,fd,iniSampleTime,dcCount,frameCount,syncPeriod;						//session id (never 0)
																		//number of players
																		//id of this player
				 														//size of packet
																		//number of redundant packets
																		//length allotted to moves in synchro data
																		//

	game_map* dGame;
	sockaddr_in mySock;
	struct sockaddr_in address[100];								//socket addressed of given id
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
		
	
/**
 * @brief returns packet header
 * 
 * @param type the packet type
 * @param packId the packet id for that session
 * @param packCount the total packets to be sent
 * @return initial header
 */
	vector<char> bufInit(packType type,int packId,int packCount);	//push the first 8 bytes acc to pack type
/**
 * @brief encodes the sync packets and puts into sync Buf
 * 
 * @param moves vector of moves
 */
	void encodeSync(vector<game_map::key_tap> moves);
/**
 * @brief encodes req packets and pushes into syncBuf
 */
	void encodeSyncReqs();
/**
 * @brief encodes game request packets
 */
	void encodeGameReqs();
/**
 * @brief encode and push into sync Buf game packets
 * 
 * @param forId a disconnected id
 */
	void encodeGame(int forId);										//encodes game state		
/**
 * @brief decodes a ync packet and pts data into sync data
 * 
 * @param pack the packet
 */
	void decodeSync(vector<char> pack);								//analyzes recieved packets and updates state
/**
 * @brief decode game packets
 * 
 * @param a pack packet
 * @param aGame game_map pointer to be updated
 */
	void decodeGame(vector<char> pack,game_map* aGame);				//updates agame with data
/**
 * @brief decode sync requests of others
 * 
 * @param pack sync req packets
 */
	void decodeSyncReq(vector<char> pack);
/**
 * @brief decodes game requests
 */
	void decodeGameReq();
/**
 * @brief decodes new ip packets
 * 
 * @param pack the packet
 */
	void decodeNewIp(vector<char> pack);
/**
 * @brief encode ips of all present players
 * @return a new Ip packet
 */
	vector<char> encodeIps();
/**
 * @brief make a simple joingame packet
 * @return a joingame packet
 */
	vector<char> encodeJoinGame();
/**
 * @brief pushes new ip packets for all other players
 * 
 * @param Ip ip of joining player
 * @param port port of the joining player
 */
	void encodeNewIp(unsigned long Ip,unsigned short port);
/**
 * @brief sends packets in syncBuf
 */	
	void sendSyncBuf();												//send contents of the buffer
/**
 * @brief decodes recieved packets
 */	
	void decodeSyncBuf();											//can contain sync data ,sync req, control
/**
 * @brief decodes game buffer cmponents
 */
	void decodeGameBuf();											//can contain game data ,game req
/**
 * @brief send game buffer to recipients
 */
	void sendGameBuf();
/**
 * @brief loads packets frm socket buffer to syncBuf
 */
	void recBuf();														//session no checked here loads all data from socket to buffer wih int field -1 
/**
 * @brief wait for delay
 */
	void wait();													//wait for time delay
/**
 * @brief desolve state incase of dc
 */
	void resolveDc();
/**
 * @brief increament data session
 */
	void incCounter();												//increaments counters
/**
 * @brief clear temporary buffers
 */
	void clearTemp();												//clears temp data for restarting process
/**
 * @brief encode and push answere syncPackets acc/ to request packets recieved
 */
	void encodeSyncAns();
/**
 * @brief start game 
 */
	void start();
/**
 * @brief check if someone is dc'd
 */
	bool isDc();
/**
 * @brief check if argument is a future session packet
 * 
 * @param packSess session id of the packet
 * @return true if it is
 */
	bool futureSession(int packSess);
/**
 * @brief print the interfaces according to ipv4 domain
 * 
 * @param domain ipv4 domain identifier
 * @return 1 if success in getting info
 */
	int print_addresses(const int domain);
	
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


	Network();
	~Network();
/**
 * @brief start mutex
 * 
 */
	pthread_mutex_t strt;
	pthread_t event;
	pthread_barrier_t b1,b2;
/**
 * @brief data thread to be called in main
 * 
 * @param x sock addr pointer cast to void pointer
 */
	void* data_thread(void* x);
	// void* event_thread(void* x);
	//vector of non human players, data recieved ***empty if disconnected
};
#endif