#include "Scene.h"
#include "imageloader.h"
// creating callbacks to the class functions as describe in
// https://stackoverflow.com/questions/3589422/using-opengl-glutdisplayfunc-within-class
Scene* currentInstance;
float _angle = 0.0;
GLuint _textureBrick, _textureDoor, _textureGrass, _textureRoof, _textureWindow, _textureSky, _textureChimney, _textureSand, _textureSnow, _textureWood;

void displaycallback()
{
	currentInstance->display();
}
void reshapecallback(GLint w, GLint h)
{
	currentInstance->reshape(w, h);
}
void keyboardcallback(unsigned char key, int x, int y)
{
	currentInstance->keyboard(key, x, y);
}
void keyboardupcallback(unsigned char key, int x, int y)
{
	currentInstance->keyboardUp(key, x, y);
}
void Specialcallback(int key, int x, int y)
{
	currentInstance->SpecialInput(key, x, y);
}
void Specialupcallback(int key, int x, int y)
{
	currentInstance->SpecialInputUp(key, x, y);
}
void timercallback(int v)
{
	currentInstance->timer(v);
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
Scene::Scene(int argc, char** argv) {
	// init glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_STENCIL);
	glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("dog world");

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	Image* image = loadBMP("snow.bmp");
	_textureSnow = loadTexture(image);

	// create drawing objects
	this->dog = new Dog("GermanShephardLowPoly.obj", 0, 0, 0, 0.5f, glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0));
//	this->floor = new Floor(-10, 10, -10, 10);
//	this->walls = new Walls(10, -10, 10, -10, 10);
	this->flashlight = new Light(GL_LIGHT0, 0, 8, 0, "Flashlight.obj", 0.2f);
	this->flashlight->towardVector = glm::vec3(0, 0, 1);
//	this->lamp = new Light(GL_LIGHT1, -9.15f, 4, 0, "sconce.obj", 1.0f, 180, 0, -9.15f, 0, 0);
//	this->lamp->object->angle = 270;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGLUT_Init();
	ImGui_ImplGLUT_InstallFuncs(); // use the imgui glut funcs
	ImGui_ImplOpenGL2_Init();

	// configure glut funcs
	::currentInstance = this;
	glutReshapeFunc(reshapecallback);
	glutDisplayFunc(displaycallback);
	glutTimerFunc(100, timercallback, 0);
	glutKeyboardFunc(keyboardcallback);
	glutKeyboardUpFunc(keyboardupcallback);
	glutSpecialFunc(Specialcallback);
	glutSpecialUpFunc(Specialupcallback);

	glutMainLoop(); // run the main loop

	// imgui cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();
}

// Handles the display callback to show what we have drawn.
void Scene::display() {
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	ImGui::Render();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, aspect, 1, 100.0); // use Perspective projection
	// set dog or camera view
//	if (dog_view) {
//		glm::vec3 dog_view_pos = dog->getViewPos();
//		glm::vec3 dog_view_targe = dog->getViewTarget();
//		gluLookAt(dog_view_pos[0], dog_view_pos[1], dog_view_pos[2],
//			dog_view_targe[0], dog_view_targe[1], dog_view_targe[2],
//			0, 1, 0);
//	}
//	else {
		gluLookAt(camera_position[0], camera_position[1], camera_position[2],
			camera_target[0], camera_target[1], camera_target[2],
			0, 1, 0);
//	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// enable opengl features
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	// add scene lights
	flashlight->addlight();

	// ambient intensivity
//	GLfloat globalAmbientVec[4] = { ambient_intensity , ambient_intensity, ambient_intensity, 1.0f };
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientVec);
//	glLoadIdentity();
	glEnable(GL_LIGHTING);
	// start drawing
//	floor->draw();
	dog->draw();
//	flashlight->draw();
    glDisable(GL_LIGHTING);
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

	// add Coordinate Arrows for debug
	drawCoordinateArrows();
	//imgui does not handle light well
//	glDisable(GL_LIGHTING);
//	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());


	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

// Handles keyboard press
void Scene::keyboard(unsigned char key, int x, int y) {
	// imgui keyboard func
	ImGui_ImplGLUT_KeyboardFunc(key, x, y);

	// do the key action
	key = tolower(key);
	if (key == 'w') {
		dog->walk(0.2f);
	}
	else if (key == 's') {
		dog->walk(-0.2f);
	}
	else if (key == 'd') {
		dog->rotate(-5.0f);
	}
	else if (key == 'a') {
		dog->rotate(5.0f);
	}
	else if (key == 'm') {
		show_menu = !show_menu;
	}
	else if (key == 'q') {
		dog->rotateOrgan(2.0f, DOG_HEAD, false);
	}
	else if (key == 'e') {
		dog->rotateOrgan(-2.0f, DOG_HEAD, false);
	}
	else if (key == 'g') {
		dog->rotateOrgan(1.0f, DOG_HEAD, true);
	}
	else if (key == 't') {
		dog->rotateOrgan(-1.0f, DOG_HEAD, true);
	}
	else if (key == 'v') {
		dog_view = !dog_view;
	}
	glutPostRedisplay();
}

// Handles keyboard after press
void Scene::keyboardUp(unsigned char key, int x, int y) {
	// imgui keyboard func
	ImGui_ImplGLUT_KeyboardUpFunc(key, x, y);

	// do the key action
	key = tolower(key);
	if (key == 'w' || key == 's') {
		close_legs_vert = true;
	}
	if (key == 'a' || key == 'd') {
		close_legs_hor = true;
	}
}


void Scene::SpecialInput(int key, int x, int y) {
	// imgui special func
	ImGui_ImplGLUT_SpecialFunc(key, x, y);

	// do the key action
	switch (key)
	{
	case GLUT_KEY_UP:
		dog->walk(0.2f);
		break;
	case GLUT_KEY_DOWN:
		dog->walk(-0.2f);
		break;
	case GLUT_KEY_LEFT:
		dog->rotate(5.0f);
		break;
	case GLUT_KEY_RIGHT:
		dog->rotate(-5.0f);
		break;
	}
}

void Scene::SpecialInputUp(int key, int x, int y) {
	// imgui specialup func
	ImGui_ImplGLUT_SpecialUpFunc(key, x, y);

	// make the legs move when walk or rotate
	if (key == GLUT_KEY_UP || key == GLUT_KEY_DOWN ) {
		close_legs_vert = true;
	}
	if (key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT) {
		close_legs_hor = true;
	}
}

void Scene::timer(int v) {
	// wag tail if needed
	if (wag_tail) {
		dog->wagTail();
		glutPostRedisplay();
	}
	// move legs if needed
	if (close_legs_vert && abs(dog->organsAngles[DOG_LEFT_BACK_LEG][true]) >= 3.0f) {
		dog->moveLegs(3.0f, true);
		glutPostRedisplay();
	}
	else
	{
		close_legs_vert = false;
	}
	// move legs if needed
	if (close_legs_hor && abs(dog->organsAngles[DOG_LEFT_BACK_LEG][false]) >= 8.0f) {
		dog->moveLegs(8.0f, false);
		glutPostRedisplay();
	}
	else
	{
		close_legs_hor = false;
	}
	glutTimerFunc(1000 / 60, timercallback, v); // recursivly call to itslef for the next frame
}

// Handles the window reshape event
void Scene::reshape(GLint w, GLint h) {
	// imgui reshape func
	ImGui_ImplGLUT_ReshapeFunc(w, h);

	glViewport(0, 0, w, h);
	aspect = float(w / h);
}

void Scene::drawCoordinateArrows(void) {
	if (!debug_mode) {
		return;
	}

	// draw lines
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glEnd();


	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glEnd();


	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 1.0);
	glEnd();

	// draw letters
	glRasterPos3f(1.2f, 0.0f, 0.0f);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'x');
	glRasterPos3f(0.0f, 1.2f, 0.0f);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'y');
	glRasterPos3f(0.0f, 0.0f, 1.2f);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'z');
}
