#include "spark.h"

spark::spark(int num_par, Vector3 centre){
		num_particles = num_par;
		timeElapsed = 0;
		position = new Vector3[num_par];
		for (int i = 0; i < num_par; i++){
			position[i].set(centre);
		}
		speed = new Vector3[num_par];
		for (int i = 0; i < num_par; i++){
			float a = float(rand()%100 - 50)/1000;
			float b = float(rand()%100 - 50)/1000;
			float c = float(rand()%100 - 50)/1000;
			speed[i].set(a,b,c);
		}
}

void spark::display(int life){
	timeElapsed++;
	for(int i = 0; i < num_particles; i++){
		glColor3f(0.4,0.9,1.0);
		position[i].set(position[i].add(speed[i]));
		speed[i] = speed[i].mult(0.93);
		//glPointSize(2);
		glBegin(GL_POINTS);
		glVertex3f(position[i].x, position[i].y, position[i].z);
		glEnd();
	}
}