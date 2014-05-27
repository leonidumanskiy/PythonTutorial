/*
Leonid Umanskiy
lumanskiy@herzing.edu
GD447: Game Programming III
*/

#include <iostream>
#include <time.h>
#include "GL\freeglut.h"
#include "Python32\include\Python.h"

using namespace std;

// Funciton prototypes
void initGL();
void initPython();
void destroyGame();
void drawCube();
void cubeClick();

// Event handlers
void display();
void update(int value);
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);

// Vars and constants for OpenGL
float eye[3] = {0, 0, 500};
float center[3] = {0, 0, 330};
float fovY = 60;
float aspectX = 1;
float zNear = 1;
float zFar = 700;
float up[3] = {0, 1, 0};
int fps = 100;
float points[8][3] =
{
	{-1, -1, 1}, {1, -1, 1}, {1, -1, -1}, {-1, -1, -1},
	{-1, 1, 1},  {1, 1, 1},  {1, 1, -1},  {-1, 1, -1},	
};

const int windowHeight = 600;
const int windowWidth = 600;
const int BOUND = 100;
const int ESC = 27;
clock_t currentTime;
clock_t lastCurrentTime;
float cubeAngle = 0;

unsigned char cubeText[255];

// Python variables
char pModuleName[] = "NPC_Cube";
char pClassName[] = "NPC_Cube";
char pFunctionName[] = "dialog";

// We store our python-based npc object here
PyObject *pCubeNpc;



//main()
//input:	int argc
//			char **argv
//output: n/a
// Program entry point
int main(int argc, char **argv)
{
	//openGL + glut initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowHeight, windowWidth);
	glutCreateWindow("OpenGL python tutorial");

	initGL();
	initPython();


	// glut functions
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(1000/fps, update, 0);
	glutMainLoop();

	destroyGame();
	return 0;
}

//initGL()
//input: n/a
//output: n/a
// Init OpenGL
void initGL()
{
	glClearColor(0.0f,0.0f,0.1f,0);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// setup the camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-BOUND, BOUND, -BOUND, BOUND, -BOUND, BOUND);

	// enbable access to the alpha channel
	glEnable(GL_BLEND);

}

//display()
//input: n/a
//output: n/a
// Display function
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovY, aspectX, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2],
			center[0], center[1], center[2],
			up[0], up[1], up[2]);

	// code to display
	drawCube();

	// Swap buffers
	glutSwapBuffers();

}



//keyboard()
//input: unsigned char key
//		 int x
//		 int y
//output: n/a
// OpenGL keyboard function
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case ESC: glutLeaveMainLoop(); break;
		default: break;
	}
	glutPostRedisplay();
}

//update()
//input: int value
//output: n/a
// Update function - updates cube angles
void update(int value)
{
	lastCurrentTime = currentTime;
	currentTime = clock();

	float deltaT = (float) difftime(currentTime, lastCurrentTime) / 1000.0f;
	float deltaTSqDiv2 = (float) (deltaT * deltaT) * 0.5f;

	cubeAngle+=0.3f;

	glutTimerFunc(1000/fps, update, 0);
	glutPostRedisplay();
}


//destroyGame()
//input: n/a
//output: n/a
// Destroy game function
void destroyGame()
{
	// Clean python cube npc instance
	Py_DECREF(pCubeNpc); 

	// Finalize python
	Py_Finalize();
}


//mouse()
//input:	int button
//			int x
//			int y
//output: n/a
// OpenGL mouse function. Registers mouse clicks on buttons
void mouse(int button, int state, int x, int y)
{
	float newX = (float) x / windowWidth * 200.0f - BOUND;
	float newY = (float) y / windowHeight * 200.0f - BOUND;
	newY = -1*newY; // No idea why is it flipped


	switch (button)
	{
		case GLUT_LEFT_BUTTON: 
		{
			if(state == GLUT_UP)
				cubeClick();
			break;
		}
		default: break;
	}
}

// Draw cube and text
void drawCube()
{
	// Draw text
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos3f(-70, -80, 330);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, cubeText);


	// Draw cube object
	glEnable(GL_DEPTH_TEST);
	float scaleSize = 100;

	glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glScalef(scaleSize,scaleSize,scaleSize);
			glRotatef(cubeAngle, 0.1f, 1.0f, 0.1f);
			glPushMatrix();
					// FRONT
					glMatrixMode(GL_MODELVIEW);
					glBegin(GL_QUADS);
						glColor3f(0.15f, 0.15f, 0.3f);
						glVertex3fv(points[2]);
						glVertex3fv(points[3]);
						glVertex3fv(points[7]);
						glVertex3fv(points[6]);
					glEnd();

		
					// BACK
					glMatrixMode(GL_TEXTURE);
					glLoadIdentity();
					glPushMatrix();
						glScalef(-1, 1, 1);

						glMatrixMode(GL_MODELVIEW);
						
						glColor3f(0.35f, 0.15f, 0.2f);
						glBegin(GL_QUADS);					
							glVertex3fv(points[0]);
							glVertex3fv(points[1]);
							glVertex3fv(points[5]);
							glVertex3fv(points[4]);
						glEnd();
						glMatrixMode(GL_TEXTURE);
					glPopMatrix();
					glMatrixMode(GL_MODELVIEW);
 
					// RIGHT		
					glColor3f(0.15f, 0.25f, 0.2f);
					glBegin(GL_QUADS);
						glVertex3fv(points[1]);
						glVertex3fv(points[2]);
						glVertex3fv(points[6]);
						glVertex3fv(points[5]);
					glEnd();



					// LEFT
					glMatrixMode(GL_TEXTURE);
					glPushMatrix();
						glScalef(1, -1, 1);
						glRotatef(90, 0,0,1);
						glMatrixMode(GL_MODELVIEW);
						glColor3f(0.25f, 0.25f, 0.2f);
						glBegin(GL_QUADS);
							glVertex3fv(points[0]);
							glVertex3fv(points[3]);
							glVertex3fv(points[7]);
							glVertex3fv(points[4]);
						glEnd();
						glMatrixMode(GL_TEXTURE);
					glPopMatrix();
 
 
					// BOTTOM

					glMatrixMode(GL_MODELVIEW);
					glColor3f(0.35f, 0.25f, 0.25f);
					glBegin(GL_QUADS);
						glVertex3fv(points[0]);
						glVertex3fv(points[1]);
						glVertex3fv(points[2]);
						glVertex3fv(points[3]);
					glEnd();

					// TOP				
					glColor3f(0.25f, 0.25f, 0.23f);
					glBegin(GL_QUADS);
						glVertex3fv(points[4]);
						glVertex3fv(points[5]);
						glVertex3fv(points[6]);
						glVertex3fv(points[7]);
					glEnd();
				glPopMatrix();	
		glPopMatrix();
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);

}

// Init python, load module / class, create instance
void initPython()
{
	Py_Initialize(); // Initialize python 

	PyObject *pName = PyUnicode_FromString(pModuleName); // Module Name
	PyObject *pModule = PyImport_Import(pName); // Import module NPC_Cube
	
	if(pModule == NULL) {
		cerr << "Failed to load module:" << endl;
		PyErr_Print();
		return;
	}

	// Create pCubeNpc python class instance by calling it
	pCubeNpc = PyObject_CallMethod(pModule, pClassName, "");
	if(pCubeNpc == NULL) {
		cerr << "Failed to create instance:" << endl;
		PyErr_Print();
		return;
	}

	// Clean memory
	Py_DECREF(pName); 
	Py_DECREF(pModule); 
}

// When user clicks cube, call python npc dialog
void cubeClick()
{
	// Call our "dialog" method with no args
	PyObject *pValue = PyObject_CallMethod(pCubeNpc, pFunctionName, "");
	if(pValue == NULL) {
		cerr << "Failed to call method" << endl;
		PyErr_Print();
		return;
	}

	// Convert python unicode object to ascii string
	PyObject *pStr = PyUnicode_AsASCIIString(pValue);
	// Convert python ascii string to C string
	const char *c_str = PyBytes_AsString(pStr);
	memcpy_s(cubeText, 255, c_str, 255);

	// Clean up
	Py_DECREF(pValue); 
	Py_DECREF(pStr); 
}
