#include "camera.h"

void camera::set(GLfloat x_, GLfloat y_, GLfloat z_, GLfloat player_x_, GLfloat player_y_, GLfloat player_z_, 
					GLfloat up_x_, GLfloat up_y_, GLfloat up_z_, GLfloat nearWidth_, GLfloat frontRelative_, 
					GLfloat backRelative_)
{
	x = x_;
	y = y_;
	z = z_;
	player_x = player_x_;
	player_y = player_y_;
	player_z = player_z_;
	up_x = up_x_;
	up_y = up_y_;
	up_z = up_z_;
	nearWidth = nearWidth_;
	frontRelative = frontRelative_;
	backRelative = backRelative_;
}