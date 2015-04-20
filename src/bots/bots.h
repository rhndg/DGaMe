#ifndef BOTS_H
#define BOTS_H

#include <bits/stdc++.h>
#include "../vector/Vector3.h"
#include "../weapons/weapons.h"

class bots
{
public:
	bots(int ID,int gr_id,Vector3 pos,int n_lives,int n_kills,int difficulty);
	
	//respawn
	bool bot_reset();

	//health boost
	void get_health_pack(int id);

	//recovery
	void health_recovery();

	//weaponry
	void get_weapon(int ID);

	//change weapon
	void toggle_weapon();

	//attack(add bullet object to bullets)
	void fire_weapon();
	
	//collision with player(collateral damage)
	int Player_CheckCollision();

	int Bot_CheckCollision();

	int CheckHit();
	
	void Player_SolveCollision();

	void Bot_CheckCollision();

	//hit by bullet(remove bullet object from bullets)
	void OnHit();

	Vector3 set_velocity(int up,int right,int move_up,int move_right,Vector3 v);
	
	void surface_constraint();

	int new_location(int move_up,int move_right);

	void update(vector<key_tap> keys_pressed);

	direction get_directions(int i);

	bool present_in_vector(int i,vector<nodes>* checkzones);

	void add_to_vector(int i,int j,int up,int right,vector<nodes>* checkzones,int level);

	void analyze_view();

	vector<int> projected_path();

	void projected_collision();

	vector<int> level_1_projected_path(int i,int j,int difficulty);

	void level_1_projected_collision(int move_up,int move_right);

	int id;	
	int group_id;				//for team play(0 for neutral)
	int g_locate;	

	/*effective motion taps*/
	int up;
	int right;

	/*bounding sphere*/
	float mass;
	float radius;
	
	~Bots();
private:
	int dificulty_level;
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
	
	//field of view parameters
	field_view field_of_view;
	vector<int> player_collision;
	vector<int> bots_collision;
	vector<int> level_1_player_collision;
	vector<int> level_1_bot_collision;
};

#endif