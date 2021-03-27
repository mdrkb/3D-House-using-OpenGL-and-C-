#include "Walls.h"

Walls::Walls(GLfloat height, GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax, int rows, int columns) {
	this->xMin = xMin;
	this->xMax = xMax;
	this->yMin = yMin;
	this->yMax = yMax;
	this->height = height;
	this->rows = rows;
	this->columns = columns;
}

void Walls::draw() {
	glPushMatrix();
	glNormal3d(0, 1, 0);

	GLfloat specular[] = { 1.0f, 1.0f, 1.0f };
	GLfloat shininess = 64.0f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	GLfloat wall_color[4] = { this->color[0], this->color[1], this->color[2], this->alpha };

	float width, row_step, column_step;
	row_step = height / (float)columns; // calc rows height
	glBegin(GL_QUADS);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wall_color);
		if (showSouth) {
			width = glm::distance(glm::vec2(xMin, yMin), glm::vec2(xMax, yMin)); // calc width, the distance between (xMin, yMin) to (xMax, yMin)
			column_step = width / (float)columns; // calc column width
			// for each row
			for (int row = 0; row < rows; row++)
			{
				// for each column
				for (int column = 0; column < columns; column++)
				{
					glVertex3f(xMin + column * column_step, row * row_step, yMin );
					glVertex3f(xMin + (column + 1) * column_step, row * row_step, yMin);
					glVertex3f(xMin + (column + 1) * column_step, (row + 1) * row_step, yMin);
					glVertex3f(xMin + column * column_step, (row + 1) * row_step, yMin);
				}
			}
		}
		if (showNorth) {
			width = glm::distance(glm::vec2(xMin, yMax), glm::vec2(xMax, yMax));
			column_step = width / (float)columns;
			for (int row = 0; row < rows; row++)
			{
				for (int column = 0; column < columns; column++)
				{
					glVertex3f(xMin + column * column_step, row * row_step, yMax);
					glVertex3f(xMin + (column + 1) * column_step, row * row_step, yMax);
					glVertex3f(xMin + (column + 1) * column_step, (row + 1) * row_step, yMax);
					glVertex3f(xMin + column * column_step, (row + 1) * row_step, yMax);
				}
			}
		}
		if (showWest) {
			width = glm::distance(glm::vec2(xMin, yMin), glm::vec2(xMin, yMax));
			column_step = width / (float)columns;
			for (int row = 0; row < rows; row++)
			{
				for (int column = 0; column < columns; column++)
				{
					glVertex3f(xMin, row * row_step, yMin + column * column_step);
					glVertex3f(xMin, row * row_step, yMin + (column + 1) * column_step);
					glVertex3f(xMin, (row + 1) * row_step, yMin + (column + 1) * column_step);
					glVertex3f(xMin, (row + 1) * row_step, yMin + column * column_step);
				}
			}
		}
		if (showEast) {
			width = glm::distance(glm::vec2(xMax, yMin), glm::vec2(xMax, yMax));
			column_step = width / (float)columns;
			for (int row = 0; row < rows; row++)
			{
				for (int column = 0; column < columns; column++)
				{
					glVertex3f(xMax, row * row_step, yMin + column * column_step);
					glVertex3f(xMax, row * row_step, yMin + (column + 1) * column_step);
					glVertex3f(xMax, (row + 1) * row_step, yMin + (column + 1) * column_step);
					glVertex3f(xMax, (row + 1) * row_step, yMin + column * column_step);
				}
			}
		}
	glEnd();

	glPopMatrix();
}