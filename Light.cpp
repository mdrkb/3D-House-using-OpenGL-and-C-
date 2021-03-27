#include "Light.h"

Light::Light(int id, GLfloat PosX, GLfloat PosY, GLfloat PosZ, string object, GLfloat scale,
		     GLfloat cutoff, GLfloat exponent,
	         GLfloat TargetX, GLfloat TargetY, GLfloat TargetZ) {
	this->id = id;
	this->position[0] = PosX;
	this->position[1] = PosY;
	this->position[2] = PosZ;
	this->position[3] = 1.0f;
	this->target[0] = TargetX;
	this->target[1] = TargetY;
	this->target[2] = TargetZ;
	this->cutoff = cutoff;
	this->exponent = exponent;

	// if given .obj file input
	if (object.length() > 0) {
		this->object = new ObjectGL(object);
		this->object->scale = scale;
	}

	enable();
}

void Light::draw() {
	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);
	fixDirection();

	if (this->object != NULL) {
		this->object->draw(); // draw the light object
	}
	else {
		// draw deafult light
		GLfloat ambient[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat diffuse[4] = { 0.01f, 0.01f, 0.01f, 1.0f };
		GLfloat specular[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
		GLfloat shininess = 32.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);

		glutSolidCone(0.3, 0.6, 10, 10);
		glPushMatrix();
		glTranslatef(0, 0, 0.1f);
		glutSolidCylinder(0.2, 0.39, 10, 10);
		glPopMatrix();

		glDisable(GL_LIGHTING);
		glColor3fv(color);
		glutSolidSphere(0.2, 100, 100);
		glEnable(GL_LIGHTING);
	}
	
	glPopMatrix();
}

void Light::disable()
{
	glDisable(id);
}

void Light::enable()
{
	glEnable(id);
}

void Light::addlight() {
	// add light only if enabled
	if (!glIsEnabled(id))
		return;

	glLightfv(id, GL_DIFFUSE, this->color);
	glLightfv(id, GL_SPECULAR, this->color);
	glLightfv(id, GL_POSITION, this->position);
	GLfloat direction[3] = { this->target[0] - this->position[0],
							 this->target[1] - this->position[1],
							 this->target[2] - this->position[2] }; // calc light direction
	glLightfv(this->id, GL_SPOT_DIRECTION, direction);
	glLightf(this->id, GL_SPOT_CUTOFF, this->cutoff);
	glLightf(this->id, GL_SPOT_EXPONENT, this->exponent);
}

void Light::fixDirection() {
	glm::vec3 eye = glm::vec3(this->position[0], this->position[1], this->position[2]); // the light pos
	glm::vec3 center = glm::vec3(this->target[0], this->target[1], this->target[2]); // the light target

	glm::vec3 wantedVector = glm::normalize(center - eye); // the light vector at the end
	glm::vec3 normal = glm::cross(towardVector, wantedVector); // normal to the plain that contain the cur vector and the new vector
	normal = glm::normalize(normal);
	float radian_angle = glm::angle(towardVector, wantedVector); // calc angle between the vectors
	float angle = radian_angle * 180 / glm::pi<float>();
	glRotatef(angle, normal.x, normal.y, normal.z); // rotate the drawing
}