#include "Floor.h"

Floor::Floor(GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax, int rows, int columns) {
	this->xMax = xMax;
	this->xMin = xMin;
	this->yMax = yMax;
	this->yMin = yMin;
	this->rows = rows;
	this->columns = columns;
}

void Floor::draw()
{
	glPushMatrix();
	glNormal3d(0, 1, 0);

	float width = xMax - xMin;
	float height = yMax - yMin;
	float row_step = height / (float)columns; // calc rows height
	float column_step = width / (float)columns; // calc column width

	GLfloat specular[] = { 1.0f, 1.0f, 1.0f };
	GLfloat shininess = 128.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess); // make the floor shiny

	glBegin(GL_QUADS);
		// for each row
		for (int row = 0; row < rows; row++)
		{
			// for each column
			for (int column = 0; column < columns; column++)
			{
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, (row + column) % 2 == 0 ? color1 : color2); // choose color
				glVertex3f(xMin + column * column_step, 0, yMin + row * row_step);
				glVertex3f(xMin + (column + 1) * column_step, 0, yMin + row * row_step);
				glVertex3f(xMin + (column + 1) * column_step, 0, yMin + (row + 1) * row_step);
				glVertex3f(xMin + column * column_step, 0, yMin + (row + 1) * row_step);
			}
		}
	glEnd();

	glPopMatrix();
}