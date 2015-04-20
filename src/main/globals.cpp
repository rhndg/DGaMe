#include "globals.h"

float DELTA_T;				//time parameter

enum key_tap{				//valid keyboard inputs(add cases of esc,pause,etc;)			
		up,down,left,right,
		rot_left,rot_right,
		shoot,toggle
};

/*health parameters*/
int health_inrease;			//health boost effect
int health_time;				//time for which a health_pack is available(multiple of DELTA_T)
float health_recovery;		//rate of recovery
float recovery_time;			//time after which recovery starts(multiple of DELTA_T)

int collateral_damage			//collateral damage on collision players/bots

int max_lives;				//no. of lives for the player in the game

/*collision parameters*/
float player_mass;
float player_radius;

vector<bullet> bullets;		//vector of bullets in the current game state
vector<grid> Map;			//grid map	
vector<weapons> Weapon_set;	//set of all weapons used in the game
float rotate;				//rotation rate
float acceleration;			//acceleration on key press
float max_speed;				//terminal speed of player(4*acceleration)
game_map* gameplay; 

struct field_view{
	vector<nodes> view_armoury;
	vector<nodes> view_boosts;
	vector<nodes> view_players;
	vector<bots> view_bots;
	vector<bullet> view_bullets;
};

struct nodes{
	int id;			//index of grid
	int right;		//moves to right
	int up;			//moves to up;	
};

struct direction{
	int tang;
	int normal;
};