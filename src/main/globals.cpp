#include "globals.h"

float DELTA_T=0.05;				//time parameter
int n=10;
// const enum key_tap{				//valid keyboard inputs(add cases of esc,pause,etc;)			
// 		up,down,left,right,
// 		rot_left,rot_right,
// 		shoot,toggle
// };


/*health parameters*/
const int health_inrease = 25;				//health boost effect
const int health_time = 10;					//time for which a health_pack is available(multiple of DELTA_T)
const int health_recovery = 10;			//rate of recovery
const int recovery_time = 1;	//time after which recovery starts(multiple of DELTA_T)

const int collateral_damage = 15;			//collateral damage on collision players/bots

int max_lives = 3;							//no. of lives for the player in the game

/*collision parameters*/
float player_mass = 1.0;
float player_radius = 2.0;

// vector<bullet> bullets;						//vector of bullets in the current game state
// vector<grid> Maps;						//grid map	
// vector<bullet*> bullet_set;	//set of all bullets corresponding to every weapon
vector<weapons> Weapon_set;					//set of all weapons used in the game
float rotate;								//rotation rate
float acceleration = 0.01;							//acceleration on key press
float max_speed = 2.0;							//terminal speed of player(4*acceleration)
game_map* gameplay;
int bullet_counter = 0;