#ifndef NODES_H
#define NODES_H

class nodes{
public:
 	nodes();
	nodes(int id_,int right_,int up_);
	int id;			//index of grid
	int right;		//moves to right
	int up;			//moves to up;	
};

#endif
