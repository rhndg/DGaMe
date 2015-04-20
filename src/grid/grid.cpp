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

int grid::check_neighbour(grid* g)
{
	int index=-1;
	for(int i=0;i<8;i++){
		if(Map[(neigh[i])]==g)
			index=i;
	}
	return index;
}

float grid::distance_from_surface(Vector3 v)
{
	Vector3 pos=v.add((vertex[0]).neg());
	return (pos.dot(normal)).mod();
}

grid::grid (float* f,int* n){
	vertex = new Vector3[4];
	draw_vertex = new Vector3[4];
	neigh = new int[4];
	vertex[0].set(f[0],f[1],f[2]);
	vertex[1].set(f[3],f[4],f[5]);
	vertex[2].set(f[6],f[7],f[8]);
	vertex[3].set(f[9],f[10],f[11]);
	neigh[0] = n[0];
	neigh[1] = n[1];
	neigh[2] = n[2];
	neigh[3] = n[3];
}

grid::~grid(){

}