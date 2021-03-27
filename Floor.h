#pragma once
#include <GL/glut.h>
#include <string>

using namespace std;

class Floor
{
public:
	Floor(GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax, int rows = 10, int columns = 10);
	void draw(); // draw the floor
	~Floor() = default;
	GLfloat xMin; // floor's left x coordinate
	GLfloat xMax; // floor's right x coordinate
	GLfloat yMin; // floor's top y coordinate
	GLfloat yMax; // floor's bottom y coordinate
	GLfloat color1[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // first color
	GLfloat color2[4] = { 0.7f, 0.7f, 0.7f, 1.0f }; // seconde color
	int rows; // number of rows to draw in the floor
	int columns; // number of columns to draw in the floor
private:
};