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
	grid(Vector3* f,int* n);
	~grid();
	Vector3* vertex;
	//Vector3* draw_vertex;
	
	//vector of ints; int is the id->index of the array of All_players
	vector<int> Players;
	vector<int> Bots;
	vector<int> Bullets;
	vector<int> Armoury;
	vector<int> Boosts;
	int* neigh;
	Vector3 normal;
	int check_neighbour(grid* g);
	Vector3 set_normal(Vector3* f);
	float distance_from_surface(Vector3 pos)
};

#endif