#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <vector>
#include <GL/glut.h>
#include <GL/glu.h>
#include "../player/player.h"
#include "../bots/bots.h"
#include "../bullet/bullet.h"
#include "../weapons/weapons.h"
#include "../vector/Vector3.h"
#include "../grid/grid.h"
#include "../health_pack/health_pack.h"
#include "../main/globals.h"

using namespace std;

class grid;
class player;
class health_pack;
class bots;
class bullet;


class game_map{
public:

	// enum key_tap{				//valid keyboard inputs(add cases of esc,pause,etc;)			
	// 	up,down,left,right,
	// 	shoot_up,shoot_right,
	// 	shoot_down,shoot_left,
	// 	toggle
	// };	

	game_map(int n);
	~game_map();
	vector<health_pack> Boosts;
	vector<grid> Maps;				//all the grid instances
	vector<bots> All_Bots;			
	vector<player> All_Players;		//All_Players[player_id] gives player info
	vector<bullet> Bullets;
	vector<weapons> Weapons;
	// vector<key_tap> KeyPressed;		//updated by keypad func
	vector<int> KeyPressed;
	void draw();
	// vector<key_tap>AI(int player_id,int movCount);
	vector<int>AI(int player_id,int movCount);
};

#endif