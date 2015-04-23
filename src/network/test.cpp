# include <bits/stdc++.h>

using namespace std;


int movNum,numPlayers,dataSession=100,controlSession,playerId=5,packSize=128,mvsLen=10	//number of moves(keytaps)
		,maxTime,minTime,sendGameTo,sockNum;
enum packType {
		syncPack,gamePack,controlPack,syncReqPack,gameReqPack
	};
enum key_tap{
		up,down,left,right,rot_left,rot_right,shoot_up,shoot_down,shoot_left,shoot_right,skill,nop
	};

vector<key_tap> nops;

vector<char> bufInit(packType t,int packId,int packCount){
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

vector<char> encodeSync(vector<key_tap> moves){
	vector<char> buffer=bufInit(syncPack,0,3);
	moves.resize(mvsLen,nop);
	buffer.push_back(((char)playerId));
	for(int i = 0;i<mvsLen;i+=2){
		int x=(int)moves[i];
		x=x*16+((int)moves[i+1]);
		buffer.push_back(((char)x));
	}
	buffer.resize(packSize,((char)-1));
	return buffer;
}

vector<key_tap> decodeSync(vector<char> data){
	int i=8,playerID;
	vector<key_tap>test;
	while(data[i]!=((char)-1)){
		playerID=data[i];
		i++;
		int x,y,j=0,k;
		while(j!=mvsLen/2){
			k=(data[i+j]+256)%256;
			x=k/16;
			y=k%16;
			test.push_back(((key_tap)x));
			test.push_back(((key_tap)y));
			j++;
		}
		i+=j;
	}
	return test;
}


void prnt (vector<char> v){
	for(int i=0;i!=v.size();i++) cout<<((int)v[i])<<" ";
	cout<<endl;
}
void prnt_(vector<key_tap>v){
	for(int i=0;i!=v.size();i++) cout<<((int)v[i])<<" ";
	cout<<endl;

}
int main(){
	nops.resize(packSize,nop);
	nops[0]=up;
	nops[2]=down;
	nops[1]=((key_tap)3);
	prnt(encodeSync(nops));
	prnt_(decodeSync(encodeSync(nops)));
}