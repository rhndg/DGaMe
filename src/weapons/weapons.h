#ifndef WEAPONS_H
#define WEAPONS_H

#include <bits/stdc++.h>
#include "../vector/Vector3.h"

using namespace std;

class weapons{
public:
	weapons(bool def,int damage_,float speed_,float timespan_,float shot_time_,int max_,int upgrade_);
	void shot_fired();
	bool no_shots();
	void upgrade_weapon();
	int ID;					//weapon ID-> increasing order of damage
private:	
	bool default_weapon		//for default weapon		
	int damage;				//range(0-100)
	float speed;			//speed of shot fired		
	float timespan;			//time for which the weapon exists with the player
	float last_shot;		//time interval after last shot of this weapon
	float shot_time;		//min. time between succesive shots
	int shots;				//no. of shots left
	int max_shots;			//max. shots for this weapon
	int upgrade;			//no. of shots to be increased
};

#endif