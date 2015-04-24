#ifndef FIELD_VIEW_H
#define FIELD_VIEW_H

#include "nodes.h"
#include "../bullet/bullet.h"

class bullet;

class field_view{
public:
	field_view();
	vector<nodes> view_armoury;
	vector<nodes> view_boosts;
	vector<nodes> view_players;
	vector<nodes> view_bots;
	vector<bullet> view_bullets;
};

#endif