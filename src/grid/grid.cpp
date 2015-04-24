#include "grid.h"

grid::grid(Vector3* f,int* n,int i)
{
	vertex = new Vector3[4];
	//draw_vertex = new Vector3[4];
	neigh = new int[8];
	for(int i=0;i<4;i++)
		vertex[i]=f[i];
	for(int i=0;i<8;i++)
		neigh[i] = n[i];
	normal = set_normal(f);
	id=i;
}

grid::grid(const grid& g)
{
	vertex = new Vector3[4];
	vertex = g.vertex;
	Players= g.Players;
	Bots= g.Bots;
	Bullets = g.Bullets;
	Armoury = g.Armoury;
	Boosts= g.Boosts;
	neigh= new int[8];
	neigh=g.neigh;
	normal= g.normal;

}

Vector3 grid::set_normal(Vector3* f)
{
	Vector3 first_edge = f[0].add(f[1].neg());
	Vector3 second_edge = f[2].add(f[1].neg());
	first_edge=first_edge.cross(second_edge);
	Vector3 comPoint = f[0].add(first_edge.setlen(0.1));
	if (comPoint.mod()>f[0].mod())
		return first_edge.setlen(1.0);
	else
		return (first_edge.setlen(1.0)).neg();
}

int grid::check_neighbour(int g)
{
	int index=-1;
	for(int i=0;i<8;i++){
		if(neigh[i]==g)
			index=i;
	}
	return index;
}

float grid::distance_from_surface(Vector3 v)
{
	Vector3 pos=v.add((vertex[0]).neg());
	 return pos.dot(normal);
}

//grid assumed to be square
bool grid::point_in_plane_inclusive(Vector3 v)
{
	float d1 = ((v.add((vertex[3]).neg())).cross(vertex[0].add(vertex[3].neg()))).mod();
	float d2 = ((v.add((vertex[0]).neg())).cross(vertex[1].add(vertex[0].neg()))).mod();
	float d3 = ((v.add((vertex[1]).neg())).cross(vertex[2].add(vertex[1].neg()))).mod();
	float d4 = ((v.add((vertex[2]).neg())).cross(vertex[3].add(vertex[2].neg()))).mod();
	
	return (d1<=1 && d2<=1 && d3<=1 && d4<=1) ;
}
bool grid::point_in_plane(Vector3 v)
{
	float d1 = ((v.add((vertex[3]).neg())).cross(vertex[0].add(vertex[3].neg()))).mod();
	float d2 = ((v.add((vertex[0]).neg())).cross(vertex[1].add(vertex[0].neg()))).mod();
	float d3 = ((v.add((vertex[1]).neg())).cross(vertex[2].add(vertex[1].neg()))).mod();
	float d4 = ((v.add((vertex[2]).neg())).cross(vertex[3].add(vertex[2].neg()))).mod();
	
	return (d1<1 && d2<1 && d3<1 && d4<1) ;
}

//adjust motion parameters so that object doesnt go beyond this frame
int grid::find_grid(Vector3 v)
{
	float distance=distance_from_surface(v);
	Vector3 position= v.add(normal.mult(-distance));
	if(point_in_plane_inclusive(position))
		return 8;
	else{
		for(int i=0;i<8;i++)
		{
			int j= neigh[i];
			grid g= gameplay->Maps[j];
			distance= g.distance_from_surface(v);
			position= v.add((g.normal).mult(-distance));
			if(g.point_in_plane(position))
				return i;
		}
		for(int i=0;i<8;i++)
		{
			int j= neigh[i];
			grid g= gameplay->Maps[j];
			distance= g.distance_from_surface(v);
			position= v.add((g.normal).mult(-distance));
			if(g.point_in_plane_inclusive(position))
				return i;
		}
		return 9;
	}
}

Vector3 grid::set_point()
{
	Vector3 edge1= vertex[1].add(vertex[0].neg());
	Vector3 first = vertex[0].add(edge1.setlen(edge1.mod()/2));
	Vector3 edge2= vertex[3].add(vertex[0].neg());
	Vector3 second =first.add(edge2.setlen(edge2.mod()/2));
	return second;
}

grid::~grid(){

}