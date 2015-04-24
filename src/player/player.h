#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include "../vector/Vector3.h"
#include "../weapons/weapons.h"
#include "../main/globals.h"
#include "../bots/bots.h"
#include "../grid/grid.h"
#include "../game_map/game_map.h"
#include "../health_pack/health_pack.h"
#include "../bullet/bullet.h"

using namespace std;

class player{
public:	
	player();
	player(int ID,int gr_id,Vector3 pos,int n_lives,int n_kills,int g);	//add mass,radius

	// player( player &obj);
	//respawn
	void player_reset();

	//health boost
	void get_health_pack(int id);

	//recovery
	void recovery();

	//weaponry
	void get_weapon(int ID);

	//change weapon
	void toggle_weapon();

	//attack(add bullet object to bullets)
	void fire_weapon(int i);
		
	//check collision
	int Player_CheckCollision();
	
	int Bot_CheckCollision();
	
	//check hit;
	// int CheckHit();

	//collision with player(collateral damage)
	void Player_SolveCollision();
	
	void Bot_SolveCollision();
	
	//hit by bullet(remove bullet object from bullets)
	void OnHit();

	Vector3 set_velocity(int up,int right,int move_up,int move_right,Vector3 v);
	
	void surface_constraint();

	int new_location(int move_up,int move_right);

	// void update(vector<key_tap> keys_pressed);
	void update(vector<int> keys_pressed);

	~player();

	string name;
	int id;	
	int group_id;				//for team play(0 for neutral)
	int g_locate;	
	/*effective motion taps*/
	int fwd;
	int tang;

	/*bounding sphere*/
	float mass;
	float radius;
	bool active;				//to check if player is active or dead
	Vector3 position;
	Vector3 velocity;
	int last_hit;				//used with timestamps; for health recovery 
private:

	
	int health;					//range(0-100); would be displayed on a progress bar
	vector<weapons> weaponry;	//first element will be the default weapon
	int lives;
	int current_weapon;
};

#endif