#ifndef BULLET_H
#define BULLET_H

#include <bits/stdc++.h>
#include "../vector/Vector3.h"
#include "../weapons/weapons.h"
#include "../effects/damage_effect.h"
#include "../effects/bullet_effect.h"

using namespace std;
class bullet{
private:

public:
	bullet();
	~bullet();

};

class bullet{
public: 
	bullet(int id,int gr_id,Weapon* weapon,Vector3 pos,Vector3 dir);

private:
	int id;
	int group_id;
	Vector3 position;
	Vector3 velocity;
	damage_effect effect;
	bullet_effect movement;
	int damage;
};

#endif