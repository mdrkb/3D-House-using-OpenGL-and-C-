#include "Scene.h"
#include "imageloader.h"

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

Scene::Scene(int argc, char** argv) {
	// init glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_STENCIL);
	glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("3D House");

	Initialize();

	// create drawing objects
	this->dog = new Dog("GermanShephardLowPoly.obj", 0, -1.5f, 1.5, 0.05f, glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0));
   this->horse = new ObjectGL("horse01.obj", -2.0, -1.5f, 2.0, 0.4f, glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0));
   this->flashlight = new Light(GL_LIGHT0, 0, 8, 0, "Flashlight.obj", 0.2f);
	this->flashlight->towardVector = glm::vec3(0, 0, 1);

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

	// set view
   gluLookAt(0, 0, 1, camera_target[0], camera_target[1], camera_target[2], 0, 1, 0);

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



    // Sky
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, _textureSky);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTranslatef(0,0,-10);
        glBegin(GL_QUADS);
            glTexCoord3f(0.0,1.0,0.1);  glVertex3f(-20,20,0);
            glTexCoord3f(1.0,1.0,0.1);  glVertex3f(20,20,0);
            glTexCoord3f(1.0,0.0,0.1);  glVertex3f(20,-20,0);
            glTexCoord3f(0.0,0.0,0.1);  glVertex3f(-20,-20,0);
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
            glTexCoord3f(2.0,0.0,-1); glVertex3f(-1.8,0.65,0.3);
            glTexCoord3f(0.0,0.0,1); glVertex3f(-1.8,0.5,0.6);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureDoor);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Door
            glTexCoord3f(0.0,1.0,1.0001); glVertex3f(-2.001,-0.4,0.3);
            glTexCoord3f(1.0,1.0,1.0001); glVertex3f(-2.001,-0.4,-0.3);
            glTexCoord3f(1.0,0.0,1.0001); glVertex3f(-2.001,-1.5,-0.3);
            glTexCoord3f(0.0,0.0,1.0001); glVertex3f(-2.001,-1.5,0.3);
        glEnd();
    glPopMatrix();

   // Objects
	glEnable(GL_LIGHTING);
      glTranslatef(0,0,-6);
      glRotatef(_angle, 0.0, 1.0, 0.0);
      dog->draw();
      horse->draw();
    glDisable(GL_LIGHTING);
//

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

// Handles keyboard press
void Scene::keyboard(unsigned char key, int x, int y) {
	// imgui keyboard func
	ImGui_ImplGLUT_KeyboardFunc(key, x, y);

	key = tolower(key);
	if (key == 'w') {
		dog->walk(0.05f);
	}
	else if (key == 's') {
		dog->walk(-0.05f);
	}
	else if (key == 'd') {
		dog->rotate(-5.0f);
	}
	else if (key == 'a') {
		dog->rotate(5.0f);
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
	glutPostRedisplay();
}

// Handles keyboard after press
void Scene::keyboardUp(unsigned char key, int x, int y) {
	// imgui keyboard func
	ImGui_ImplGLUT_KeyboardUpFunc(key, x, y);

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
//	switch (key)
//	{
//	case GLUT_KEY_UP:
//		dog->walk(0.05f);
//		break;
//	case GLUT_KEY_DOWN:
//		dog->walk(-0.05f);
//		break;
//	case GLUT_KEY_LEFT:
//		dog->rotate(5.0f);
//		break;
//	case GLUT_KEY_RIGHT:
//		dog->rotate(-5.0f);
//		break;
//	}
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
	aspect = float(w) / float(h);
}
