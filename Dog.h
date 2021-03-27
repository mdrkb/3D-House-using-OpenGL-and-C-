#pragma once
using namespace std;
#include "ObjectGL.h"

// organs macro
#define DOG_HEAD			0
#define DOG_TAIL			1
#define DOG_LEFT_BACK_LEG   2
#define DOG_RIGHT_BACK_LEG  3
#define DOG_LEFT_FRONT_LEG  4
#define DOG_RIGHT_FRONT_LEG 5
#define DOG_EYES            6

// this class represnt a dog given as a .obj file
class Dog : public ObjectGL {
	//using ObjectGL::ObjectGL;
	public:
		Dog(string inputfile, GLfloat PosX = 0, GLfloat PosY = 0, GLfloat PosZ = 0, GLfloat scale = 1.0f,
			glm::vec3 upVector = glm::vec3(0, 1, 0), glm::vec3 towardVector = glm::vec3(0, 0, 0), GLfloat angle = 0);
		// map organ to it's relative posistion
		map<int, vector<GLfloat>> organsPos = {
		//      organ			                 pos
			{DOG_HEAD,            { -2.4644f, 4.9088f, 0.0f      } },
			{DOG_TAIL,            { 2.9906f,  3.8998f, 0.0f      } },
			{DOG_LEFT_BACK_LEG,   { 1.8531f,  3.6308f, 1.0157f   } },
			{DOG_RIGHT_BACK_LEG,  { 1.8531f,  3.6308f, -1.0157f  } },
			{DOG_LEFT_FRONT_LEG,  { -1.185f,  2.2522f, 0.78169f  } },
			{DOG_RIGHT_FRONT_LEG, { -1.185f,  2.2522f, -0.78169f } },
			{DOG_EYES,			  { -3.3146f,  5.897f, 0.0f      } }
		};
		// map organ to it's max angle
		map<int, map<bool, float>> maxOrgansAngles = {
	    //    organ			         vertical max   horizontal max
			{DOG_HEAD,            { {true, 30.0f}, {false, 60.0f} } },
			{DOG_TAIL,            { {true, 30.0f}, {false, 30.0f} } },
			{DOG_LEFT_BACK_LEG,   { {true, 30.0f}, {false, 40.0f} } },
			{DOG_RIGHT_BACK_LEG,  { {true, 30.0f}, {false, 40.0f} } },
			{DOG_LEFT_FRONT_LEG,  { {true, 30.0f}, {false, 40.0f} } },
			{DOG_RIGHT_FRONT_LEG, { {true, 30.0f}, {false, 40.0f} } }
		};
		// map organ to it's current angle
		map<int, map<bool, float>> organsAngles = {
		//    organ			        vertical max   horizontal max
			{DOG_HEAD,            { {true, 0.0f}, {false, 0.0f} } },
			{DOG_TAIL,            { {true, 0.0f}, {false, 0.0f} } },
			{DOG_LEFT_BACK_LEG,   { {true, 0.0f}, {false, 0.0f} } },
			{DOG_RIGHT_BACK_LEG,  { {true, 0.0f}, {false, 0.0f} } },
			{DOG_LEFT_FRONT_LEG,  { {true, 0.0f}, {false, 0.0f} } },
			{DOG_RIGHT_FRONT_LEG, { {true, 0.0f}, {false, 0.0f} } }
		};
		void rotateOrgan(GLfloat angle, int organ, bool vertical); // rotate specific organ (head, tail, etc..)
		void walk(GLfloat distance); // make the dog walk
		void rotate(GLfloat angle); // rotate the dog
		void wagTail(GLfloat angle = 5.0f); // wag the dog's tail
		void moveLegs(GLfloat angle, bool vertical = true); // move the dog's legs (for walking)
		glm::vec3 getViewPos(); // get the pos of the dog's eyes
		glm::vec3 getViewTarget(); // get the viewing target 
		bool right_legs_forward = true; // legs movement direction
		bool legs_turn_right = true; // legs rotate direction
	private:
		glm::vec3 sideVector; // the currnet side vector
		glm::vec3 orignalSideVector; // the begging side vector
		bool tail_turn_right = true; // tail turn direction
		GLfloat legs_angle_per_step = 3.0f; // legs walking movement speed
		GLfloat legs_angle_per_rotate = 8.0f; // legs rotation movement speed
		void handleRotation(int organ); // roatate an organ from a task
		glm::vec3 getViewVector(); // get the viewing vector 
};