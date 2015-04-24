#ifndef WEAPONS_H
#define WEAPONS_H

#include "../vector/Vector3.h"

using namespace std;

class weapons{
public:
	weapons(bool def,int damage_,float speed_,int timespan_,int shot_time_,int max,int upgrade_,int ID);
	void shot_fired();
	bool no_shots();
	void upgrade_weapon();
	int ID;					//weapon ID-> increasing order of damage
	int last_shot;		//time interval after last shot of this weapon
	int shot_time;		//min. time between succesive shots
	int damage;				//range(0-100)
	float speed;
	int timespan;			//time for which the weapon exists with the player
private:	
	bool default_weapon;	//for default weapon		
			//speed of shot fired		

	int shots;				//no. of shots left
	int max_shots;			//max. shots for this weapon
	int upgrade;			//no. of shots to be increased
};

#endif