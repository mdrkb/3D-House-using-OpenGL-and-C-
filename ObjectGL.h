#pragma once

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <tiny_obj_loader.h>
#include <stb_image.h>
#include <functional>
#include <vector>
#include <map>

using namespace std;

bool FileExists(const std::string& abs_filename); //check if a file exists in the given path

string GetBaseDir(const std::string& filepath); // get the base directory of a file

const string OBJECTS_DIR = "objects"; // the deafult directory of the .obj files
const string TEXTURES_DIR = "textures"; // the deafult directory of the textures files

// this class handle drawing objects given by .obj files
class ObjectGL {
	protected:
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes; // the shapes that make the object
		std::vector<tinyobj::material_t> materials; // the object materials
		map<string, GLuint> textures; // map texture file name to it's opengl texture id
	public:
		ObjectGL(string inputfile, GLfloat PosX = 0, GLfloat PosY = 0, GLfloat PosZ = 0, GLfloat scale = 1.0f,
			     glm::vec3 upVector = glm::vec3(0, 1, 0), glm::vec3 towardVector = glm::vec3(0, 0, 0), GLfloat angle = 0);
		ObjectGL() = default;
		string inputfile; // the .obj file defining the object
		GLfloat PosX; // the x object position 
		GLfloat PosZ; // the z object position 
		GLfloat PosY; // the y object position 
		GLfloat angle; // the angle of the object
		GLfloat scale; // the scale of the object
		glm::vec3 towardVector; // where the object "look" (use for movement)
		glm::vec3 upVector; // the up direction
		map<string, vector<function<void()>>> shapesTasks; // drawing tasks add to specific shape (or to the whole object)
		void draw(); // draw the object
		void setPosition(GLfloat x, GLfloat y, GLfloat z); // set the position of the object
		void rotate(GLfloat angle); // rotate the object
		void addTask(function<void()> func, string shape = "GLOBAL"); // add task shapesTasks
		void walk(GLfloat distance); // move the object foreward
		static GLuint create_texture(string texture_filename); // create opengl texture and return it's id
};