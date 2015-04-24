#ifndef BOTS_H
#define BOTS_H

#include <vector>
#include <stack>
#include <math.h>
#include <iostream>
#include "../vector/Vector3.h"
#include "../weapons/weapons.h"
#include "../main/globals.h"
#include "../player/player.h"
#include "../bullet/bullet.h"
#include "../game_map/game_map.h"
#include "../health_pack/health_pack.h"
#include "../grid/grid.h"
#include "../extras/directions.h"
#include "../extras/field_view.h"
#include "../extras/nodes.h"

using namespace std;

class field_view;

class bots
{
public:
	bots();

	bots(int ID,int gr_id,Vector3 pos,int n_lives,int difficulty,int g);
	
	bots(const bots& b);

	//respawn
	void bot_reset();

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
	
	//collision with player(collateral damage)
	int Player_CheckCollision();

	int Bots_CheckCollision();

	// int CheckHit();
	
	void Player_SolveCollision();

	void Bots_SolveCollision();
	//hit by bullet(remove bullet object from bullets)
	void OnHit();

	Vector3 set_velocity(int up,int right,int move_up,int move_right,Vector3 v);
	
	void surface_constraint();

	int new_location(int move_up,int move_right);

	void update();

	direction* get_directions(int i);

	bool present_in_vector(int i);

	void add_to_vector(int i,int j,int up,int right,int level);

	void analyze_view();

	vector<int> projected_path(int i,int j, int difficulty);

	void projected_collision();

	vector<int> level_1_projected_path(int i,int j,int difficulty);

	void level_1_projected_collision(int move_up,int move_right);

	vector<int> projected_bullet(bullet b,int difficulty);

	vector<int> level_1_projected_bullet(bullet b, int difficulty);

	vector<int> projected_hit(vector<int> path);

	vector<int> projected_shoot(int i);

	bool search_boosts();

	bool search_armoury();

	bool dodge();

	bool dodgebullet();

	void attack();

	bool wander();

	void stack_change();

	int id;	
	int group_id;				//for team play(0 for neutral)
	int g_locate;	
	/*effective motion taps*/
	int fwd;
	int tang;
	/*bounding sphere*/
	float mass;
	float radius;
	Vector3 position;
	Vector3 velocity;
	bool active;				//to check if player is active or dead
	int health;					//range(0-100); would be displayed on a progress bar
	//field of view parameters
	field_view* field_of_view;
	int last_hit;				//used with timestamps; for health recovery 
	~bots();
private:
	int difficulty_level;
	vector<weapons> weaponry;	//first element will be the default weapon
	//Vector3 aim;				//direction of firing shot
//	int kills;
	int lives;
	int current_weapon;
	
	vector<nodes> checkzones;
	
	stack <string> state_stack;
	vector<int> player_collision;
	vector<int> bots_collision;
	vector<int> bullet_collision;
	vector<int> level_1_player_collision;
	vector<int> level_1_bots_collision;
	vector<int> level_1_bullet_collision;
	vector<int> ans;
};

#endif