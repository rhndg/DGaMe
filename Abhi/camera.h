#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glut.h> 
class camera{
	public:
		GLfloat x;
		GLfloat y;
		GLfloat z;
		GLfloat player_x;
		GLfloat player_y;
		GLfloat player_z;
		GLfloat up_x;
		GLfloat up_y;
		GLfloat up_z;
		GLfloat nearWidth;
		GLfloat frontRelative;
		GLfloat backRelative;
		void set(GLfloat x_, GLfloat y_, GLfloat z_, GLfloat player_x_, GLfloat player_y_, GLfloat player_z_, 
					GLfloat up_x_, GLfloat up_y_, GLfloat up_z_, GLfloat nearWidth, GLfloat frontRelative_, 
					GLfloat backRelative_);
};

#endif