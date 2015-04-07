#include "grid.h"

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