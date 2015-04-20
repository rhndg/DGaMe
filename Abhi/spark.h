#ifndef SPARK_H
#define SPARK_H

#include "Vector3.h"
#include <time.h>
#include <GL/glut.h>

class spark{
	public:
		int num_particles;
		int timeElapsed;
		Vector3* position;
		Vector3* speed;
		spark(int num_par, Vector3 center);
		void display(int life);
};

#endif