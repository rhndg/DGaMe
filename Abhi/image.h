#ifndef image_H
#define image_H
#include <GL/glut.h>

class image{
	public:
		GLuint texture;
		double width;
		double height;
		void set(double w, double h);
};

#endif