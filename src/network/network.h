#ifndef NETWORK_H
#define NETWORK_H

#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
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
	int movNum,NumPlayers,Session,PlayerId,packSize,packCount;	//number of moves(keytaps)session id (never 0),number of players, id of this player
	map< int,vector < pair<int,sockaddr_in> > > address;		//socket addressed of given id
	map< int,bool > IsPeer;										//current connected peers
	map< int,vector <int> > Request;							//requested data of players by the given id
	vector< vector<game_map::key_tap> data;						//recieved data
	vector<char> buffer;										//buffer
	float delay;												//time delay for message to reach
	void analyze();												//analyzes recieved packets and updates state
	void construct(int Id);										//constructs packets to be sent as per requests 
	void sendbuf(int Id);										//send contents of the buffer to the specified id
	void wait();												//wait for time delay
	void clearTemp();											//clears temp data for restarting process
public:
	Network();
	Network(string host);
	~Network();
	vector< vector<game_map::key_tap> > Synchronize(vector<game_map::key_tap> v);
	//vector of non human players, data recieved ***empty if disconnected
};
#endif