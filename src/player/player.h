#ifndef PLAYER_H
#define PLAYER_H

#include <bits/stdc++.h>
#include "../vector/Vector3.h"
#include "../weapons/weapons.h"
#include "../main/globals.h"
#include "../grid/grid.h"
#include "../main/game_map.h"

using namespace std;

class player{
public:	
	player(int ID,int gr_id,Vector3 pos,int n_lives,int n_kills);	//add mass,radius

	//respawn
	bool player_reset();

	//health boost
	void get_health_pack(int id);

	//recovery
	void recovery();

	//weaponry
	void get_weapon(int ID);

	//change weapon
	void toggle_weapon();

	//attack(add bullet object to bullets)
	void fire_weapon();
		
	//check collision
	int Player_CheckCollision();
	
	int Bot_CheckCollision();
	
	//check hit;
	int CheckHit();

	//collision with player(collateral damage)
	void Player_SolveCollision();
	
	void Bot_SolveCollision();
	
	//hit by bullet(remove bullet object from bullets)
	void OnHit();

	Vector3 set_velocity(int up,int right,int move_up,int move_right,Vector3 v);
	
	void surface_constraint();

	int new_location(int move_up,int move_right);

	void update(vector<key_tap> keys_pressed);

	~player();

	int id;	
	int group_id;				//for team play(0 for neutral)
	int g_locate;	
	/*effective motion taps*/
	int up;
	int right;

	/*bounding sphere*/
	float mass;
	float radius;

private:

	bool active;				//to check if player is active or dead
	int health;					//range(0-100); would be displayed on a progress bar
	vector<weapons> weaponry;	//first element will be the default weapon
	Vector3 position;
	Vector3 velocity;
	Vector3 aim;				//direction of firing shot
//	int kills;
	int lives;
	float last_hit;				//used with timestamps; for health recovery 
	int current_weapon;
};

#endif