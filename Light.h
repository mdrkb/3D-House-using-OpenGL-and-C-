#pragma once
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/vector_angle.hpp >
#include <algorithm>
#include <iterator>
using namespace std;

#include "ObjectGL.h"

class Light {
public:
	GLfloat position[4]; // the position of the light
	GLfloat target[3]; // where the light point to
	GLfloat color[3] = { 1.0f, 1.0f, 1.0f }; // the color of the light
	GLfloat cutoff; // the angle that the light is affective
	GLfloat exponent; // the intensity distribution of the light
	glm::vec3 towardVector = glm::vec3(0, -1, 0);  // the direction of the light drawing

	ObjectGL* object = NULL; // object to draw

	Light(int id, GLfloat PosX = 0, GLfloat PosY = 10, GLfloat PosZ = 0, string object = "",
		  GLfloat scale = 1.0f, GLfloat cutoff = 90.0f, GLfloat exponent = 0.0f,
		  GLfloat TargetX = 0, GLfloat TargetY = 0, GLfloat TargetZ = 0);
	void draw(); // draw the light in the scene
	void addlight(); // add the lighting of the light
	void disable(); // enable the light
	void enable(); // disable the light
	~Light() = default;
private:
	int id; // must be GL_LIGHTi where 0 <= i < GL_MAX_LIGHTS
	void fixDirection(); // rotate the light drawing to the wanted direction (from position to target)
};