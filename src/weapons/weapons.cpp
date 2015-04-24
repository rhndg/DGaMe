#include "weapons.h"

weapons::weapons(bool default_weapon_,int damage_,float speed_,int timespan_,int shot_time_,int max,int upgrade_,int id)
{
	default_weapon=default_weapon_;
	damage=damage_;
	speed=speed_;
	timespan=timespan_;
	shot_time=shot_time_;
	if(default_weapon)
		shots=max;
	upgrade=upgrade_;
	ID = id;
	last_shot = 0;
}
void weapons::shot_fired()
{
	if((!default_weapon) && (last_shot>shot_time))
	{
		shots--;
		last_shot=0;
	}
}
bool weapons::no_shots()
{
	return (shots==0);
}
void weapons::upgrade_weapon()
{
	if(shots<max_shots-upgrade)
		shots+=upgrade;
	else
		shots=max_shots;
}
