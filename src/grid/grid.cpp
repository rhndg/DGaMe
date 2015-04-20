#include "grid.h"

grid::grid (Vertex3* f,int* n){
	vertex = new Vector3[4];
	//draw_vertex = new Vector3[4];
	neigh = new int[8];
	for(int i=0;i<4;i++)
		vertex[i]=f[i];
	for(int i=0;i<8;i++)
		neigh[i] = n[i];
	normal = set_normal(f);
}

Vector3 grid::set_normal(Vector3* f)
{
	Vector3 first_edge = f[0].add(f[1].neg());
	Vector3 second_edge = f[2].add(f[1].neg());
	first_edge=first_edge.cross(second_edge);
	return first_edge.setlen(1.0);
}

int check_neighbour(grid* g)
{
	int index=-1;
	for(int i=0;i<8;i++){
		if(Map[(neigh[i])]==g)
			index=i;
	}
	return index;
}

float distance_from_surface(Vector3 v)
{
	Vector3 pos=v.add((vertex[0]).neg());
	return (pos.dot(normal)).mod();	
}

grid::~grid(){

}