#include <stdlib.h>
#include <windows.h>
#include <GL/glut.h>
#include "imageloader.h"

float _angle = 0.0;
GLuint _textureBrick, _textureDoor, _textureGrass, _textureRoof, _textureWindow, _textureSky, _textureChimney, _textureSand, _textureSnow, _textureWood;

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);

    // Sky
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, _textureSky);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTranslatef(0,0,-10);
        glBegin(GL_QUADS);
            glTexCoord3f(0.0,1.0,0.1);  glVertex3f(-10,10,0);
            glTexCoord3f(1.0,1.0,0.1);  glVertex3f(10,10,0);
            glTexCoord3f(1.0,0.0,0.1);  glVertex3f(10,-10,0);
            glTexCoord3f(0.0,0.0,0.1);  glVertex3f(-10,-10,0);
        glEnd();
    glPopMatrix();

    // Snow
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, _textureSnow);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTranslatef(0,0,-6);
        glRotatef(_angle, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);
            glTexCoord3f(0.0,70.0,1);  glVertex3f(-50,-1.5,50);
            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-50,-1.5,-50);
            glTexCoord3f(70.0,0.0,-1);  glVertex3f(50,-1.5,-50);
            glTexCoord3f(70.0,70.0,1);  glVertex3f(50,-1.5,50);
        glEnd();
    glPopMatrix();

    // Front side
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, _textureWood);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTranslatef(0,0,-6);
        glRotatef(_angle, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);  // Wall
            glTexCoord3f(0.0,2.0,0.1);  glVertex3f(-2,0.1,1);
            glTexCoord3f(4.0,2.0,0.1);  glVertex3f(2,0.1,1);
            glTexCoord3f(4.0,0.0,0.1);  glVertex3f(2,-1.5,1);
            glTexCoord3f(0.0,0.0,0.1);  glVertex3f(-2,-1.5,1);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureChimney);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Chimney
            glTexCoord3f(0.0,2.0,0.1);  glVertex3f(-1.8,1.2,0.6);
            glTexCoord3f(4.0,2.0,0.1);  glVertex3f(-1.5,1.2,0.6);
            glTexCoord3f(4.0,0.0,0.1);  glVertex3f(-1.5,0.5,0.6);
            glTexCoord3f(0.0,0.0,0.1);  glVertex3f(-1.8,0.5,0.6);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureSnow);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Roof
            glTexCoord3f(0.0,2.0,0); glVertex3f(-2.2,1.0001,0);
            glTexCoord3f(4.0,2.0,0);glVertex3f(2.2,1.0001,0);
            glTexCoord3f(4.0,0.0,1.25); glVertex3f(2.2,0.0,1.25);
            glTexCoord3f(0.0,0.0,1.25); glVertex3f(-2.2,0.0,1.25);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureWood);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Roof Lower
            glTexCoord3f(0.0,2.0,0); glVertex3f(-2.2,1.0,0);
            glTexCoord3f(4.0,2.0,0);glVertex3f(2.2,1.0,0);
            glTexCoord3f(4.0,0.0,1.25); glVertex3f(2.2,0.0,1.25);
            glTexCoord3f(0.0,0.0,1.25); glVertex3f(-2.2,0.0,1.25);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureDoor);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Door
            glTexCoord3f(0.0,1.0,1.0001); glVertex3f(0.75,-0.4,1.0001);
            glTexCoord3f(1.0,1.0,1.0001); glVertex3f(1.5,-0.4,1.0001);
            glTexCoord3f(1.0,0.0,1.0001); glVertex3f(1.5,-1.5,1.0001);
            glTexCoord3f(0.0,0.0,1.0001); glVertex3f(0.75,-1.5,1.0001);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureWindow);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Window Left
            glTexCoord3f(0.0,1.0,1.0001); glVertex3f(-1.5,-0.4,1.0001);
            glTexCoord3f(1.0,1.0,1.0001); glVertex3f(-0.75,-0.4,1.0001);
            glTexCoord3f(1.0,0.0,1.0001); glVertex3f(-0.75,-0.9,1.0001);
            glTexCoord3f(0.0,0.0,1.0001); glVertex3f(-1.5,-0.9,1.0001);
        glEnd();

        glBegin(GL_QUADS);  // Window Right
            glTexCoord3f(0.0,1.0,1.0001); glVertex3f(0.3,-0.4,1.0001);
            glTexCoord3f(1.0,1.0,1.0001); glVertex3f(-0.3,-0.4,1.0001);
            glTexCoord3f(1.0,0.0,1.0001); glVertex3f(-0.3,-0.9,1.0001);
            glTexCoord3f(0.0,0.0,1.0001); glVertex3f(0.3,-0.9,1.0001);
        glEnd();
    glPopMatrix();

    // Back side
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureWood);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTranslatef(0,0,-6);
        glRotatef(_angle, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);  // Wall
            glTexCoord3f(0.0,2.0,-1);  glVertex3f(-2,0.1,-1);
            glTexCoord3f(4.0,2.0,-1);  glVertex3f(2,0.1,-1);
            glTexCoord3f(4.0,0.0,-1);  glVertex3f(2,-1.5,-1);
            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-2,-1.5,-1);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureChimney);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Chimney
            glTexCoord3f(0.0,2.0,0.1);  glVertex3f(-1.8,1.2,0.3);
            glTexCoord3f(4.0,2.0,0.1);  glVertex3f(-1.5,1.2,0.3);
            glTexCoord3f(4.0,0.0,0.1);  glVertex3f(-1.5,0.8,0.3);
            glTexCoord3f(0.0,0.0,0.1);  glVertex3f(-1.8,0.8,0.3);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureSnow);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Roof
            glTexCoord3f(0.0,2.0,0); glVertex3f(-2.2,1.0001,0);
            glTexCoord3f(4.0,2.0,0);glVertex3f(2.2,1.0001,0);
            glTexCoord3f(4.0,0.0,-1.25); glVertex3f(2.2,0.0,-1.25);
            glTexCoord3f(0.0,0.0,-1.25); glVertex3f(-2.2,0.0,-1.25);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureWood);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Roof Lower
            glTexCoord3f(0.0,2.0,0); glVertex3f(-2.2,1.0,0);
            glTexCoord3f(4.0,2.0,0);glVertex3f(2.2,1.0,0);
            glTexCoord3f(4.0,0.0,-1.25); glVertex3f(2.2,0.0,-1.25);
            glTexCoord3f(0.0,0.0,-1.25); glVertex3f(-2.2,0.0,-1.25);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureWindow);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Window Left
            glTexCoord3f(0.0,1.0,-1.0001); glVertex3f(-1.5,-0.3,-1.0001);
            glTexCoord3f(1.0,1.0,-1.0001); glVertex3f(-0.75,-0.3,-1.0001);
            glTexCoord3f(1.0,0.0,-1.0001); glVertex3f(-0.75,-0.8,-1.0001);
            glTexCoord3f(0.0,0.0,-1.0001); glVertex3f(-1.5,-0.8,-1.0001);
        glEnd();

        glBegin(GL_QUADS);  // Window Right
            glTexCoord3f(0.0,1.0,1.0001); glVertex3f(1.5,-0.3,-1.0001);
            glTexCoord3f(1.0,1.0,1.0001); glVertex3f(0.75,-0.3,-1.0001);
            glTexCoord3f(1.0,0.0,1.0001); glVertex3f(0.75,-0.8,-1.0001);
            glTexCoord3f(0.0,0.0,1.0001); glVertex3f(1.5,-0.8,-1.0001);
        glEnd();
    glPopMatrix();

    // Right side
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, _textureWood);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTranslatef(0,0,-6);
        glRotatef(_angle, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);  // Wall
            glTexCoord3f(0.0,2.0,1); glVertex3f(2,0.2,1);
            glTexCoord3f(2.0,2.0,-1); glVertex3f(2,0.2,-1);
            glTexCoord3f(2.0,0.0,-1); glVertex3f(2,-1.5,-1);
            glTexCoord3f(0.0,0.0,1); glVertex3f(2,-1.5,1);
        glEnd();

        glBegin(GL_TRIANGLES);  // Wall Upper
            glTexCoord3f(0.0,1.0,0); glVertex3f(2,1.0,0);
            glTexCoord3f(1.0,0.0,1); glVertex3f(2,0.2,1);
            glTexCoord3f(-1.0,0.0,-1); glVertex3f(2,0.2,-1);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureChimney);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Chimney
            glTexCoord3f(0.0,2.0,1); glVertex3f(-1.5,1.2,0.6);
            glTexCoord3f(2.0,2.0,-1); glVertex3f(-1.5,1.2,0.3);
            glTexCoord3f(2.0,0.0,-1); glVertex3f(-1.5,0.5,0.3);
            glTexCoord3f(0.0,0.0,1); glVertex3f(-1.5,0.5,0.6);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureDoor);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Door
            glTexCoord3f(0.0,1.0,1.0001); glVertex3f(2.001,-0.4,0.3);
            glTexCoord3f(1.0,1.0,1.0001); glVertex3f(2.001,-0.4,-0.3);
            glTexCoord3f(1.0,0.0,1.0001); glVertex3f(2.001,-1.5,-0.3);
            glTexCoord3f(0.0,0.0,1.0001); glVertex3f(2.001,-1.5,0.3);
        glEnd();
    glPopMatrix();

    // Left side
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, _textureWood);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTranslatef(0,0,-6);
        glRotatef(_angle, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);  // Wall
            glTexCoord3f(0.0,2.0,1);    glVertex3f(-2,0.2,1);
            glTexCoord3f(2.0,2.0,-1);    glVertex3f(-2,0.2,-1);
            glTexCoord3f(2.0,0.0,-1);    glVertex3f(-2,-1.5,-1);
            glTexCoord3f(0.0,0.0,1);    glVertex3f(-2,-1.5,1);
        glEnd();

        glBegin(GL_TRIANGLES);  // Wall Upper
            glTexCoord3f(0.0,1.0,0);    glVertex3f(-2,1.0,0);
            glTexCoord3f(1.0,0.0,1);    glVertex3f(-2,0.2,1);
            glTexCoord3f(-1.0,0.0,-1);    glVertex3f(-2,0.2,-1);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureChimney);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Chimney
            glTexCoord3f(0.0,2.0,1); glVertex3f(-1.8,1.2,0.6);
            glTexCoord3f(2.0,2.0,-1); glVertex3f(-1.8,1.2,0.3);
            glTexCoord3f(2.0,0.0,-1); glVertex3f(-1.8,0.85,0.3);
            glTexCoord3f(0.0,0.0,1); glVertex3f(-1.8,0.7,0.6);
        glEnd();
    glPopMatrix();

    glutSwapBuffers();
}

void mySpecialFunc(int key, int x, int y){
	switch (key) {
    case GLUT_KEY_RIGHT:
        _angle += 1;
        if (_angle > 360) _angle = 0.0;
		break;
    case GLUT_KEY_LEFT:
        _angle -= 1;
        if (_angle > 360) _angle = 0.0;
	    break;
	}
	glutPostRedisplay();
}

GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

void Initialize() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	Image* image = loadBMP("bricks.bmp");
	_textureBrick = loadTexture(image);
	image = loadBMP("door.bmp");
	_textureDoor = loadTexture(image);
	image = loadBMP("grass.bmp");
	_textureGrass = loadTexture(image);
	image = loadBMP("roof.bmp");
	_textureRoof = loadTexture(image);
	image = loadBMP("window.bmp");
	_textureWindow = loadTexture(image);
	image = loadBMP("sky.bmp");
	_textureSky = loadTexture(image);
   image = loadBMP("chimney.bmp");
	_textureChimney = loadTexture(image);
   image = loadBMP("sand.bmp");
	_textureSand = loadTexture(image);
   image = loadBMP("snow.bmp");
	_textureSnow = loadTexture(image);
   image = loadBMP("wood.bmp");
	_textureWood = loadTexture(image);
	delete image;
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(600,600);
	glutCreateWindow("Textured House by Mai");
	glEnable(GL_DEPTH_TEST);

	glutReshapeFunc(resize);
	glutSpecialFunc(mySpecialFunc);
	glutDisplayFunc(renderScene);
	Initialize();

	glutMainLoop();

	return 0;
}
