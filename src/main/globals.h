#ifndef GLOBAL_H
#define GLOBAL_H

#include <bits/stdc++.h>
#include <pthread.h>
#include "../game_map/game_map.h"
#include "../bullet/bullet.h"
#include "../weapons/weapons.h"

using namespace std;

extern float DELTA_T;				//time parameter

extern enum key_tap{				//valid keyboard inputs(add cases of esc,pause,etc;)			
		up,down,left,right,
		rot_left,rot_right,
		shoot,toggle
};

/*health parameters*/
extern int health_inrease;			//health boost effect
extern int health_time;				//time for which a health_pack is available(multiple of DELTA_T)
extern float health_recovery;		//rate of recovery
extern float recovery_time;			//time after which recovery starts(multiple of DELTA_T)

extern collateral_damage			//collateral damage on collision players/bots

extern int max_lives;				//no. of lives for the player in the game

/*collision parameters*/
extern float player_mass;
extern float player_radius;

extern vector<bullet> bullets;		//vector of bullets in the current game state
extern vector<grid> Map;			//grid map	
extern vector<weapons> Weapon_set;	//set of all weapons used in the game
extern float rotate;				//rotation rate
extern float acceleration;			//acceleration on key press
extern float max_speed;				//terminal speed of player(4*acceleration)
extern game_map* gameplay; 

extern struct field_view{
	vector<nodes> view_armoury;
	vector<nodes> view_boosts;
	vector<nodes> view_players;
	vector<bots> view_bots;
	vector<bullet> view_bullets;
};

extern struct nodes{
	int id;			//index of grid
	int right;		//moves to right
	int up;			//moves to up;	
};

extern struct direction{
	int tang;
	int normal;
};

#endif
