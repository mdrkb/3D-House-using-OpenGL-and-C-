#include "Dog.h"

Dog::Dog(string inputfile, GLfloat PosX, GLfloat PosY, GLfloat PosZ, GLfloat scale, glm::vec3 upVector, glm::vec3 towardVector, GLfloat angle) :
	ObjectGL(inputfile, PosX, PosY, PosZ, scale, upVector, towardVector, angle) {
	this->orignalSideVector = glm::cross(this->upVector, this->towardVector);
	this->sideVector = glm::vec3(orignalSideVector.x, orignalSideVector.y, orignalSideVector.z);
	string shape;
	glEnable(GL_LIGHTING);
	// for each shape check if it's one of the dogs wanted organs and add the wanted task (to rotate this organ)
	for (size_t s = 0; s < this->shapes.size(); s++) {
		shape = this->shapes[s].name;
		if (shape.find("head") != std::string::npos) {
			this->addTask([this]() { handleRotation(DOG_HEAD); }, shape);
			continue;
		}
		if (shape.find("tail") != std::string::npos) {
			this->addTask([this]() { handleRotation(DOG_TAIL); }, shape);
			continue;
		}
		if (shape.find("leg") != std::string::npos) {
			if (shape.find("left") != std::string::npos) {
				if (shape.find("front") != std::string::npos) {
					this->addTask([this]() { handleRotation(DOG_LEFT_FRONT_LEG); }, shape);
					continue;
				}
				else if (shape.find("back") != std::string::npos) {
					this->addTask([this]() { handleRotation(DOG_LEFT_BACK_LEG); }, shape);
					continue;
				}
			}
			else if (shape.find("right") != std::string::npos) {
				if (shape.find("front") != std::string::npos) {
					this->addTask([this]() { handleRotation(DOG_RIGHT_FRONT_LEG); }, shape);
					continue;
				}
				else if (shape.find("back") != std::string::npos) {
					this->addTask([this]() { handleRotation(DOG_RIGHT_BACK_LEG); }, shape);
					continue;
				}
			}
		}
	}
}

void Dog::rotateOrgan(GLfloat angle, int organ, bool vertical) {
	// check if reaching max angle
	if (abs(organsAngles[organ][vertical] + angle) <= maxOrgansAngles[organ][vertical]) {
		organsAngles[organ][vertical] += angle;
	}
}

void Dog::handleRotation(int organ) {
	// translate to (0,0,0) for rotating and then back to orignal pos
	glTranslatef(organsPos[organ][0], organsPos[organ][1], organsPos[organ][2]);
	glRotatef(organsAngles[organ][false], this->upVector.x, this->upVector.y, this->upVector.z);
	glRotatef(organsAngles[organ][true], orignalSideVector.x, orignalSideVector.y, orignalSideVector.z);
	glTranslatef(-organsPos[organ][0], -organsPos[organ][1], -organsPos[organ][2]);
}

void Dog::walk(GLfloat distance) {
	moveLegs(legs_angle_per_step, true); // move legs for walking animation
	ObjectGL::walk(distance);
}

void Dog::rotate(GLfloat angle) {
	moveLegs(legs_angle_per_rotate, false); // move legs for walking animation
	ObjectGL::rotate(angle);
	this->sideVector = glm::cross(this->upVector, this->towardVector); // update sideVector
}

void Dog::wagTail(GLfloat angle) {
	// if reach max angle
	if (abs(organsAngles[DOG_TAIL][false]) + angle > maxOrgansAngles[DOG_TAIL][false]) {
		tail_turn_right = !tail_turn_right; // change rotation direction
	}
	if (tail_turn_right) {
		rotateOrgan(-angle, DOG_TAIL, false);
	}
	else {
		rotateOrgan(angle, DOG_TAIL, false);
	}
}

void Dog::moveLegs(GLfloat angle, bool vertical) {
	if (vertical) {
		if (abs(organsAngles[DOG_RIGHT_FRONT_LEG][vertical]) + angle > maxOrgansAngles[DOG_RIGHT_FRONT_LEG][vertical]) {
			right_legs_forward = !right_legs_forward; // change rotation direction
		}
		if (right_legs_forward) {
			rotateOrgan(-angle, DOG_RIGHT_BACK_LEG, vertical);
			rotateOrgan(-angle, DOG_RIGHT_FRONT_LEG, vertical);
			rotateOrgan(angle, DOG_LEFT_BACK_LEG, vertical);
			rotateOrgan(angle, DOG_LEFT_FRONT_LEG, vertical);
		}
		else {
			rotateOrgan(angle, DOG_RIGHT_BACK_LEG, vertical);
			rotateOrgan(angle, DOG_RIGHT_FRONT_LEG, vertical);
			rotateOrgan(-angle, DOG_LEFT_BACK_LEG, vertical);
			rotateOrgan(-angle, DOG_LEFT_FRONT_LEG, vertical);
		}
	}
	else {
		if (abs(organsAngles[DOG_RIGHT_FRONT_LEG][vertical]) + angle > maxOrgansAngles[DOG_RIGHT_FRONT_LEG][vertical]) {
			legs_turn_right = !legs_turn_right;
		}
		if (legs_turn_right) {
			rotateOrgan(angle, DOG_RIGHT_BACK_LEG, vertical);
			rotateOrgan(angle, DOG_RIGHT_FRONT_LEG, vertical);
			rotateOrgan(angle, DOG_LEFT_BACK_LEG, vertical);
			rotateOrgan(angle, DOG_LEFT_FRONT_LEG, vertical);
		}
		else {
			rotateOrgan(-angle, DOG_RIGHT_BACK_LEG, vertical);
			rotateOrgan(-angle, DOG_RIGHT_FRONT_LEG, vertical);
			rotateOrgan(-angle, DOG_LEFT_BACK_LEG, vertical);
			rotateOrgan(-angle, DOG_LEFT_FRONT_LEG, vertical);
		}
	}
}

glm::vec3 Dog::getViewPos() {
	glm::vec3 eyes = glm::vec3(organsPos[DOG_EYES][0] * scale, organsPos[DOG_EYES][1] * scale, organsPos[DOG_EYES][2] * scale); // model eyes pos * scale
	float rad_angle = (angle / 180) * glm::pi<float>(); // use radians
	glm::mat4 rotationMat(1);
	rotationMat = glm::rotate(rotationMat, rad_angle, this->upVector);
	eyes = glm::vec3(rotationMat * glm::vec4(eyes, 1.0)); // rotate the eyes
	eyes += glm::vec3(PosX, PosY, PosZ); // add cur pos
	return eyes;
}

glm::vec3 Dog::getViewTarget() {
	glm::vec3 view_vector = getViewVector();
	glm::vec3 eyes = getViewPos();
	return eyes + view_vector; // target - eyes pos = view vector => target = view vector + eyes pos
}

glm::vec3 Dog::getViewVector() {
	glm::vec3 view_vector = towardVector;

	float rad_angle = (organsAngles[DOG_HEAD][true] / 180) * glm::pi<float>(); // use radians
	glm::mat4 rotationMatVert(1);
	rotationMatVert = glm::rotate(rotationMatVert, rad_angle, this->sideVector);
	view_vector = glm::vec3(rotationMatVert * glm::vec4(view_vector, 1.0)); // rotate the eyes vertical

	rad_angle = (organsAngles[DOG_HEAD][false] / 180) * glm::pi<float>();
	glm::mat4 rotationMatHor(1);
	rotationMatHor = glm::rotate(rotationMatHor, rad_angle, this->upVector);
	view_vector = glm::vec3(rotationMatHor * glm::vec4(view_vector, 1.0)); // rotate the eyes vertical hotizontal
	return glm::normalize(view_vector);
}
