#ifndef GRID_H
#define GRID_H

#include <vector>
#include "../player/player.h"
#include "../bullet/bullet.h"
#include "../vector/Vector3.h"
#include "../health_pack/health_pack.h"

using namespace std;

class bullet;

class grid{
public:
	grid();
	grid(Vector3* f,int* n,int i);
	grid(const grid& g);
	~grid();
	Vector3* vertex;
	int id;
	//vector of ints; int is the (id->index of the array of All_players)
	
	vector<int> Players;
	vector<int> Bots;
	vector<bullet> Bullets;
	vector<int> Armoury;
	vector<int> Boosts;
	int* neigh;
	Vector3 normal;
	int check_neighbour(int g);
	Vector3 set_normal(Vector3* f);
	float distance_from_surface(Vector3 pos);
	bool point_in_plane(Vector3 v);
	bool point_in_plane_inclusive(Vector3 v);
	int find_grid(Vector3 v);
	Vector3 set_point();
};

#endif