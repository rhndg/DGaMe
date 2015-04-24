#ifndef BULLET_H
#define BULLET_H

#include <iostream>
#include <vector>
#include "../vector/Vector3.h"
#include "../weapons/weapons.h"
#include "../main/globals.h"
// #include "../effects/damage_effect.h"
// #include "../effects/bullet_effect.h"

using namespace std;


class bullet{

public: 
	bullet();
	bullet(int player_ID_,int player_GroupID_,int weapon_ID_,Vector3 position_,Vector3 dir,int g_locate_,Vector3 normal_);
	void update();
	void surface_constraint();
	int player_ID;
	int player_GroupID;
	int weapon_ID;
	Vector3 position;
	Vector3 velocity;
	int g_locate;
	Vector3 normal;
	//damage_effect effect;
	//bullet_effect movement;
	int damage;
	float duration;
	int bullet_id;

};

#endif
