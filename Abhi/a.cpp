#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "Vector3.h"
#include <SFML/Audio.hpp>
#include <time.h>
#include "spark.h"
#include <math.h>     // Needed for sin, cos
#include "lodepng.h"
#include "image.h"
#include "camera.h"

using namespace std;
#define PI 3.14159265f

 
/* Global variables */
char title[] = "3D Shapes";
camera cam;

GLfloat player_x = 1.0;
GLfloat player_y = 0.0;
GLfloat player_z = 0.0;
GLfloat player_up_x = 0.0;
GLfloat player_up_y = 0.0;
GLfloat player_up_z = 1.0;
GLdouble cube_size = 2.0;

int num_frames = 0;
int hitDuration = -1;

spark* coll;
image loadedImgs[11];

sf::Sound sound;


void loadBMP(const char * imagepath, int imageNo){

// Data read from the header of the BMP file
unsigned char header[54]; // Each BMP file begins by a 54-bytes header
unsigned int dataPos;     // Position in the file where the actual data begins
unsigned int width, height;
unsigned int imageSize;   // = width*height*3
// Actual RGB data
unsigned char * data;
// Open the file
FILE * file = fopen(imagepath,"rb");
if (!file) {printf("Image could not be opened\n"); return;}


if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
printf("Not a correct BMP file\n");
return;
}

if ( header[0]!='B' || header[1]!='M' ){
printf("Not a correct BMP file\n");
return;
}

// Read ints from the byte array
dataPos    = *(int*)&(header[0x0A]);
imageSize  = *(int*)&(header[0x22]);
width      = *(int*)&(header[0x12]);
height     = *(int*)&(header[0x16]);
int w = width; cout << "w" << w << endl;
int h = height; cout << "h" << h << endl;
int imsize = imageSize; cout << "imageSize" << imsize <<endl;
// Some BMP files are misformatted, guess missing information
if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
if (dataPos==0)      dataPos=54; // The BMP header is done that way


// Create a buffer
data = new unsigned char [imageSize];

// Read the actual data from the file into the buffer
fread(data,1,imageSize,file);

//Everything is in memory now, the file can be closed
fclose(file);

/*
unsigned int numPixels = imageSize/3;
unsigned int texture_size = numPixels*4;
GLubyte* RGBA_pixels = new GLubyte[texture_size];
for ( int i=0; i<numPixels; i++){
RGBA_pixels[i*4] = data[i*3+2];
RGBA_pixels[i*4+1] = data[i*3+1];
RGBA_pixels[i*4+2] = data[i*3];

if ( data[i*3] == 0 && data[i*3+1] == 0 && data[i*3+2] == 0 )
	RGBA_pixels[i*4+3] = 0;
else
	RGBA_pixels[i*4+3] = 255;
}
*/
// Create one OpenGL texture
// Give the name(memory location) to the texture
loadedImgs[imageNo].set(1.0,1.0);
glGenTextures(1, &(loadedImgs[imageNo].texture));
// cout << textureID << endl;
// "Bind" the newly created texture : all future texture functions will modify this texture
glBindTexture(GL_TEXTURE_2D, loadedImgs[imageNo].texture);
/*
The following line tells OpenGL the texture will be a 2D texture (GL_TEXTURE_2D). Zero represents the images level of detail, 
this is usually left at zero. Three is the number of data components. Because the image is made up of red data, green data and 
blue data, there are three components. TextureImage[0]->sizeX is the width of the texture. If you know the width, you can put it 
here, but it's easier to let the computer figure it out for you. TextureImage[0]->sizey is the height of the texture. zero is the 
border. It's usually left at zero. GL_RGB tells OpenGL the image data we are using is made up of red, green and blue data in that 
order. GL_UNSIGNED_BYTE means the data that makes up the image is made up of unsigned bytes, and finally... TextureImage[0]->data 
tells OpenGL where to get the texture data from. In this case it points to the data stored in the TextureImage[0] record.
// Generate The Texture
glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
*/
// Give the image to OpenGL (assign pixels to the texture)
glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

cout << "Texture Loaded" << endl;
}

int loadPNG(const char* imagepath, int imageNo){
	// Load file and decode image.
  std::vector<unsigned char> image;
  unsigned width, height;
  unsigned error = lodepng::decode(image, width, height, imagepath);

  // If there's an error, display it.
  if(error != 0)
  {
    std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
    return 1;
  }

   // Texture size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
  size_t u2 = 1; while(u2 < width) u2 *= 2;
  size_t v2 = 1; while(v2 < height) v2 *= 2;
  // Ratio for power of two version compared to actual version, to render the non power of two image with proper size.
  double u3 = (double)width / u2;
  double v3 = (double)height / v2;

  // Make power of two version of the image.
  std::vector<unsigned char> image2(u2 * v2 * 4);
  for(size_t y = 0; y < height; y++)
  for(size_t x = 0; x < width; x++)
  for(size_t c = 0; c < 4; c++)
  {
    image2[4 * u2 * y + 4 * x + c] = image[4 * width * y + 4 * x + c];
  }

  loadedImgs[imageNo].set(u3,v3);
  glGenTextures(1, &(loadedImgs[imageNo].texture));
  glBindTexture(GL_TEXTURE_2D, loadedImgs[imageNo].texture);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, u2, v2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image2[0]);

  // glBegin(GL_QUADS);
  //     glTexCoord2d( 0,  0); glVertex2f(    0,      0);
  //     glTexCoord2d(u3,  0); glVertex2f(width,      0);
  //     glTexCoord2d(u3, v3); glVertex2f(width, height);
  //     glTexCoord2d( 0, v3); glVertex2f(    0, height);
  // glEnd();

}

void drawSq(Vector3 tl, Vector3 bl, Vector3 br, Vector3 tr){
	Vector3 edge1(tl.x-bl.x,tl.y-bl.y,tl.z-bl.z);
	Vector3 edge2(br.x-bl.x,br.y-bl.y,br.z-bl.z);
	Vector3 normal = edge1.cross(edge2);
	normal.setlen(1);
	Vector3 cam1(cam.x,cam.y,cam.z);
	cam1.setlen(1);
	//glNormal3d(normal.x, normal.y, normal.z);
	
	glColor3f(0.1,0.6,0.8);
	//GLfloat cyan[] = {0.4f, 0.f, 1.0f, 1.f};
	//GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.f};
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cyan);
	// glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, black);
	// glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
	// glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, black);
	glBegin(GL_QUADS);
	glVertex3f(tl.x,tl.y,tl.z);
	glVertex3f(bl.x,bl.y,bl.z);
	glVertex3f(br.x,br.y,br.z);
	glVertex3f(tr.x,tr.y,tr.z);
	glEnd();

	float cam_norm_dot_inv = 1-normal.dot(cam1);
	float cam_norm_dot_inv_sq = 1-(normal.dot(cam1)*normal.dot(cam1));

	glColor3f(0+cam_norm_dot_inv*20, 15+cam_norm_dot_inv*120, 30+cam_norm_dot_inv*150);
	glBegin(GL_QUADS);
	glVertex3f((tl.x)*0.99,(tl.y)*0.99,(tl.z)*0.99);
	glVertex3f((bl.x)*0.99,(bl.y)*0.99,(bl.z)*0.99);
	glVertex3f((br.x)*0.99,(br.y)*0.99,(br.z)*0.99);
	glVertex3f((tr.x)*0.99,(tr.y)*0.99,(tr.z)*0.99);
	glEnd();
}

void drawImg(Vector3 tl, Vector3 bl, Vector3 br, Vector3 tr, int imageNo){

	glEnable(GL_TEXTURE_2D);                        // Enable Texture Mapping ( NEW )

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	/*
	The next two lines tell OpenGL what type of filtering to use when the image is larger (GL_TEXTURE_MAG_FILTER) or stretched on the 
	screen than the original texture, or when it's smaller (GL_TEXTURE_MIN_FILTER) on the screen than the actual texture. I usually 
	use GL_LINEAR for both. This makes the texture look smooth way in the distance, and when it's up close to the screen. Using GL_LINEAR 
	requires alot of work from the processor/video card, so if your system is slow, you might want to use GL_NEAREST. A texture that's 
	filtered with GL_NEAREST will appear blocky when it's stretched. You can also try a combination of both. Make it filter things up 
	close, but not things in the distance.
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE );

	glBindTexture(GL_TEXTURE_2D, loadedImgs[imageNo].texture);
	
	glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(bl.x, bl.y, bl.z);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(loadedImgs[imageNo].width, 0.0f); glVertex3f(br.x, br.y, br.z);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(loadedImgs[imageNo].width, loadedImgs[imageNo].height); glVertex3f(tr.x, tr.y, tr.z);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, loadedImgs[imageNo].height); glVertex3f(tl.x, tl.y, tl.z);  // Top Left Of The Texture and Quad
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawBullet(Vector3 direc, Vector3 normal, Vector3 pos, int imageNo, float size){
	Vector3 otherDirec = direc.cross(normal);
	direc = direc.setlen(size/2);
	otherDirec = otherDirec.setlen(size/2);
	Vector3 temp(pos.add(direc));
	Vector3 tr(temp.add(otherDirec));
	Vector3 br(temp.sub(otherDirec));
	direc = direc.neg();
	temp = pos.add(direc);
	Vector3 bl(temp.sub(otherDirec));
	Vector3 tl(temp.add(otherDirec));
	drawImg(tl,bl,br,tr,imageNo);
}


void drawPl(Vector3 frontDirec, Vector3 centre_b, Vector3 normal, float size, int imageNo, bool isHit){
	if (isHit) hitDuration=0; 
	if (hitDuration==30) hitDuration=-1;
	if (hitDuration>=0) hitDuration++;
	if(num_frames==0 && hitDuration>=0) size*=1.03;
	Vector3 sideDirec = frontDirec.cross(normal);
	sideDirec = sideDirec.setlen(0.5*size);
	frontDirec = frontDirec.setlen(0.5*size);
	Vector3 frontDirec_small = frontDirec.setlen(0.3*size);
	normal = normal.setlen(0.5*size);
	//lower vertices
	Vector3 front_b = centre_b.add(frontDirec);
	Vector3 left_b = centre_b.add(sideDirec);
	left_b = left_b.add(frontDirec_small.neg());
	Vector3 right_b = centre_b.add(sideDirec.neg());
	right_b = right_b.add(frontDirec_small.neg());
	//upper vertices
	Vector3 front_t = front_b.add(normal);
	Vector3 left_t = left_b.add(normal);
	Vector3 right_t = right_b.add(normal);
	Vector3 centre_t = centre_b.add(normal);

	//glColor3f(0.2,0.5,1.0);
	// glBegin(GL_QUADS);
	// glVertex3f(front_b.x,front_b.y,front_b.z);
	// glVertex3f(front_t.x,front_t.y,front_t.z);
	// glVertex3f(left_t.x,left_t.y,left_t.z);
	// glVertex3f(left_b.x,left_b.y,left_b.z);
	// glEnd();
	// glClear(GL_STENCIL_BUFFER_BIT);
	// glEnable(GL_STENCIL_TEST); //Enable using the stencil buffer
 //    glColorMask(0, 0, 0, 0); //Disable drawing colors to the screen
 //    glDisable(GL_DEPTH_TEST); //Disable depth testing
 //    glStencilFunc(GL_ALWAYS, 1, 1); //Make the stencil test always pass
 //    //Make pixels in the stencil buffer be set to 1 when the stencil test passes
 //    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
 //    //Set all of the pixels covered by the floor to be 1 in the stencil buffer
 //    glStencilMask(0xFF); // Enable writing to the stencil buffer
	drawImg(front_b,front_t,right_t,right_b,imageNo);
	drawImg(front_b,left_b,centre_b,right_b,imageNo);
	drawImg(centre_b,centre_t,left_t,left_b,imageNo);
	drawImg(centre_b,centre_t,right_t,right_b,imageNo);
	drawImg(front_b,front_t,left_t,left_b,imageNo);
	drawImg(front_t,left_t,centre_t,right_t,imageNo);

	// glBegin(GL_QUADS);
	// glVertex3f(front_b.x,front_b.y,front_b.z);
	// glVertex3f(front_t.x,front_t.y,front_t.z);
	// glVertex3f(right_t.x,right_t.y,right_t.z);
	// glVertex3f(right_b.x,right_b.y,right_b.z);
	// glEnd();
	
	// glBegin(GL_QUADS);
	// glVertex3f(centre_b.x,centre_b.y,centre_b.z);
	// glVertex3f(centre_t.x,centre_t.y,centre_t.z);
	// glVertex3f(left_t.x,left_t.y,left_t.z);
	// glVertex3f(left_b.x,left_b.y,left_b.z);
	// glEnd();

	// glBegin(GL_QUADS);
	// glVertex3f(centre_b.x,centre_b.y,centre_b.z);
	// glVertex3f(centre_t.x,centre_t.y,front_t.z);
	// glVertex3f(right_t.x,right_t.y,right_t.z);
	// glVertex3f(right_b.x,right_b.y,right_b.z);
	// glEnd();
	
	// glBegin(GL_QUADS);
	// glVertex3f(front_b.x,front_b.y,front_b.z);
	// glVertex3f(left_b.x,left_b.y,left_b.z);
	// glVertex3f(centre_b.x,centre_b.y,centre_b.z);
	// glVertex3f(right_b.x,right_b.y,right_b.z);
	// glEnd();

	// glBegin(GL_QUADS);
	// glVertex3f(front_t.x,front_t.y,front_t.z);
	// glVertex3f(left_t.x,left_t.y,left_t.z);
	// glVertex3f(centre_t.x,centre_t.y,centre_t.z);
	// glVertex3f(right_t.x,right_t.y,right_t.z);
	// glEnd();

	// glColorMask(1, 1, 1, 1); //Enable drawing colors to the screen
 //    glEnable(GL_DEPTH_TEST); //Enable depth testing
 //    //Make the stencil test pass only when the pixel is 1 in the stencil buffer
 //    glStencilFunc(GL_NOTEQUAL, 1, 1);
 //    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Make the stencil buffer not change
 //    glStencilMask(0); // Disable writing to the stencil buffer
    
 //    //Draw the cube, reflected vertically, at all pixels where the stencil
 //    //buffer is 1

 //    glBegin(GL_QUADS);
 //    glVertex3f(0.0f,2.0f,2.0f);
 //    glVertex3f(0.0f,-2.0f,2.0f);
 //    glVertex3f(0.0f,-2.0f,-2.0f);
 //    glVertex3f(0.0f,2.0f,-2.0f);
 //    glEnd();
 //    glDisable(GL_STENCIL_TEST);
    
 //    drawImg(front_b,front_t,right_t,right_b);
	// drawImg(front_b,left_b,centre_b,right_b);
	// drawImg(centre_b,centre_t,left_t,left_b);
	// drawImg(centre_b,centre_t,right_t,right_b);
	// drawImg(front_b,front_t,left_t,left_b);
	// drawImg(front_t,left_t,centre_t,right_t);	
    
}

void healthBar(int health_percent){
	Vector3 cam2player(cam.player_x-cam.x, cam.player_y-cam.y, cam.player_z-cam.z);
			cam2player = cam2player.setlen(cam.frontRelative);
	Vector3 camUp(cam.up_x,cam.up_y,cam.up_z);
	Vector3 horizontal = camUp.cross(cam2player);
	Vector3 vertical = camUp;
	Vector3 camEye(cam.x,cam.y,cam.z);
	Vector3 frontplaneCentre = camEye.add(cam2player);
	Vector3 healthBarCentre = frontplaneCentre.add(horizontal.setlen(0.9*cam.nearWidth));	
	
	horizontal = horizontal.setlen(0.07*cam.nearWidth);
	vertical = vertical.setlen(0.7*cam.nearWidth);
	Vector3 verticalh = vertical.setlen((-0.7+health_percent*1.4/100)*cam.nearWidth);
	Vector3 offset = vertical.add(horizontal);
	Vector3 offseth = verticalh.add(horizontal);
	Vector3 temp1 = healthBarCentre.add(offset);
	Vector3 temp1h = healthBarCentre.add(offseth);

	horizontal = horizontal.setlen(-0.07*cam.nearWidth);
	offset = vertical.add(horizontal);
	offseth = verticalh.add(horizontal);
	Vector3 temp2 = healthBarCentre.add(offset);
	Vector3 temp2h = healthBarCentre.add(offseth);

	vertical = vertical.setlen(-0.7*cam.nearWidth);
	offset = vertical.add(horizontal);
	Vector3 temp3 = healthBarCentre.add(offset);

	horizontal = horizontal.setlen(-0.07*cam.nearWidth);
	offset = vertical.add(horizontal);
	Vector3 temp4 = healthBarCentre.add(offset);

	drawImg(temp1,temp2,temp3,temp4,2);
	drawImg(temp1h,temp2h,temp3,temp4,3);
	

}

/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   // glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   // glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glEnable(GL_BLEND);        // Enable blending
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
   /*
   glEnable(GL_LIGHTING);
   	GLfloat lightpos[] = {1., 0., 0., 1.};
   	GLfloat dif[] = {1., 1., 1.};
   	GLfloat amb[] = {0., 0., 0.};
	glLightfv(GL_LIGHT1, GL_POSITION, lightpos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
	glEnable(GL_LIGHT1);
	*/
}

/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {

	num_frames++;
	if(num_frames==5) num_frames=0;

	GLfloat dist = sqrt(player_x*player_x + player_y*player_y + player_z*player_z);
   	GLfloat near = (cube_size*0.1)/dist;
   	cam.set(2*player_x,2*player_y,2*player_z,player_x,player_y,player_z,
   		player_up_x,player_up_y,player_up_z,near,
   		0.1,4*dist);
	
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   	glLoadIdentity();             // Reset
   	glFrustum(-cam.nearWidth, cam.nearWidth, -cam.nearWidth, cam.nearWidth, cam.frontRelative, cam.backRelative);

  	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

	glLoadIdentity();                 // Reset the model-view matrix
	
	gluLookAt(cam.x,cam.y,cam.z,cam.player_x,cam.player_y,cam.player_z,cam.up_x,cam.up_y,cam.up_z);
	
	Vector3 tl(0.0,1.0,1.0);
	Vector3 bl(0.0,1.0,-1.0);
	Vector3 br(0.0,-1.0,-1.0);
	Vector3 tr(0.0,-1.0,1.0);
	//drawSq(tl,bl,br,tr);
	Vector3 tl1(0.0f, -0.5f, 0.5f);  // Top Left Of The Texture and Quad
    Vector3 bl1(0.0f, -0.5f, -0.5f);  // Bottom Left Of The Texture and Quad
    Vector3 br1(0.0f, 0.5f, -0.5f);  // Bottom Right Of The Texture and Quad
    Vector3 tr1(0.0f, 0.5f, 0.5f);  // Top Right Of The Texture and Quad
    //drawImg(tl1,bl1,br1,tr1,4);
    //drawImg(br1,tr1,tl1,bl1,1);
    healthBar(100);
    
/*
    int detail = 100;
    float detail_r = 1/float(detail);
    float radius = 1;
    float detail_r_cube = detail_r * detail_r * detail_r;
    for (int i = 0; i < detail; i++){
    	glColor4f(0.5,0.9,1.0,0.01+0.01*detail_r_cube*i*i*i);
    	glutSolidSphere(radius*(1-detail_r*i),100,100);
    }
 */
   //  glDisable(GL_BLEND);        // Enable blending
   // glEnable(GL_DEPTH_TEST);  // Need to disable depth testing

	Vector3 frontDirec(0.0,1.0,0.0);
	Vector3 centre(0.0,0.0,0.0);
	Vector3 normal(1.0,0.0,1.0);
	drawPl(frontDirec, centre, normal, 1.0, 0,true);

	Vector3 normal1(1.0,0.0,0.0);
	drawBullet(frontDirec, normal1, centre, 7, 1.0);

	if ((*coll).timeElapsed < 30)
		(*coll).display(30);

/*
	glColor4f(1.0,1.0,1.0,0.5);
	glBegin(GL_TRIANGLE_FAN);
      glVertex3f(0.0f, 0.0f, 0.0f);       // Center of circle
      int numSegments = 100;
      GLfloat angle;
      for (int i = 0; i <= numSegments; i++) { // Last vertex same as first vertex
         angle = i * 2.0f * PI / numSegments;  // 360 deg for all segments
         glVertex3f(0.0f, cos(angle), sin(angle));
      }
   glEnd();
*/
	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}
 
/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);

   Vector3 centre(0.0,0.0,0.0);
   coll = new spark(500,centre);
}

void keyboard (unsigned char key, int x, int y) {
	if (key == 'f') sound.play();
}
 
void timer(int value) {
   glutPostRedisplay();
   glutTimerFunc(60, timer, 0);
}


/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	srand (time(NULL));
	sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("sound.wav"))
        return -1;
    sound.setBuffer(buffer);
    
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(640, 480);   // Set the window's initial width & height
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glutCreateWindow(title);          // Create window with the given title
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   glutTimerFunc(0,timer,0);
   glutKeyboardFunc (keyboard);
   initGL();                       // Our own OpenGL initialization
   loadBMP("/home/abhi19gupta/Desktop/COP290_ass3/DGaMe/Abhi/images/images_13_.bmp",0);
   loadPNG("/home/abhi19gupta/Desktop/COP290_ass3/DGaMe/Abhi/images/trans_7.png",1);
   loadPNG("/home/abhi19gupta/Desktop/COP290_ass3/DGaMe/Abhi/images/healthrect.png",2);
   loadPNG("/home/abhi19gupta/Desktop/COP290_ass3/DGaMe/Abhi/images/healthrectsolid2.png",3);
   loadPNG("/home/abhi19gupta/Desktop/COP290_ass3/DGaMe/Abhi/images/powers1.png",4);
   loadPNG("/home/abhi19gupta/Desktop/COP290_ass3/DGaMe/Abhi/images/powers2.png",5);
   loadPNG("/home/abhi19gupta/Desktop/COP290_ass3/DGaMe/Abhi/images/powers3.png",6);
   loadPNG("/home/abhi19gupta/Desktop/COP290_ass3/DGaMe/Abhi/images/bullet1.png",7);
   loadPNG("/home/abhi19gupta/Desktop/COP290_ass3/DGaMe/Abhi/images/bullet2.png",8);
   loadPNG("/home/abhi19gupta/Desktop/COP290_ass3/DGaMe/Abhi/images/bullet3.png",9);
   loadPNG("/home/abhi19gupta/Desktop/COP290_ass3/DGaMe/Abhi/images/bullet7.png",10);
   glutMainLoop();                 // Enter the infinite event-processing loop
   return 0;
}