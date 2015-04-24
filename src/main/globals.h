#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
#include <pthread.h>
#include "../game_map/game_map.h"
#include "../bullet/bullet.h"
#include "../weapons/weapons.h"
#include "../grid/grid.h"


using namespace std;

class bullet;
class grid;
class game_map;

extern int n; // n should be even

extern float DELTA_T;				//time parameter

/*health parameters*/
extern const int health_inrease;			//health boost effect
extern const int health_time;				//time for which a health_pack is available(multiple of DELTA_T)
extern const int health_recovery;		//rate of recovery
extern const int recovery_time;			//time after which recovery starts(multiple of DELTA_T)

extern const int collateral_damage;			//collateral damage on collision players/bots

extern int max_lives;				//no. of lives for the player in the game

/*collision parameters*/
extern float player_mass;
extern float player_radius;

// extern vector<bullet> bullets;		//vector of bullets in the current game state

// extern vector<grid> Maps;			//grid map	
extern vector<weapons> Weapon_set;	//set of all weapons used in the game
// extern vector<bullet*> bullet_set;	//set of all bullets corresponding to every weapon

extern float rotate;				//rotation rate
extern float acceleration;			//acceleration on key press
extern float max_speed;				//terminal speed of player(4*acceleration)
extern game_map* gameplay; 

extern int bullet_counter;
/* keyboard inputs*/

/* 0- up
 * 1- right
 * 2- down
 * 3- left
 * 4- shoot_up
 * 5- shoot_right
 * 6- shoot_down
 * 7- shoot_left
 * 8- toggle*/	
#endif
