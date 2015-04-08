#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <bits/stdc++.h>
#include "../player/player.h"
#include "../bullet/bullet.h"
#include "../game_map/game_map.h"
#include "../vector/Vector3.h"
#include "../grid/grid.h"

using namespace std;
class game_map{
private:

public:
	game_map();
	~game_map();
	
	enum key_tap{
		up,down,left,right,rot_left,rot_right,shoot,skill
	};
	vector<grid> Map;				//all the grid instances
	vector<player> All_Players;		//All_Players[player_id] gives player info
	vector<key_tap> KeyPressed;		//updated by keypad func
	void draw();
	vector<key_tap> AI (int player_id);
	void update(vector< vector<key_tap> > playerIn);
	
};

#endif