#ifndef GRID_H
#define GRID_H

#include <bits/stdc++.h>
#include "../player/player.h"
#include "../bullet/bullet.h"
#include "../vector/Vector3.h"

using namespace std;

class grid{
private:

public:
	grid(float* f,int* n);
	~grid();
	Vector3* vertex;
	Vector3* draw_vertex;
	vector<player*> Players;
	vector<bullet> Bullets;
	int* neigh;
};

#endif