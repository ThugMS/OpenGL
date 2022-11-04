#include <vgl.h>
#include <InitShader.h>
#include <vec.h>
#include "MyPlane.h"

#define Q 113
#define W 119
#define NUM_1 49
#define NUM_2 50
#define SPACEBAR 32

float gTime = 0.0f;
GLuint prog;
MyColorPlane plane;
bool gWave = 0;
bool isTurn = false;

void init()
{
	plane.init();
	prog = InitShader("vShader.glsl", "fShader.glsl");
	glUseProgram(prog);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	plane.draw(prog, gTime, gWave);
	glFlush();
}

void myIdle()
{
	if (isTurn) {
		gTime += 0.001f;
		glutPostRedisplay();
	}
}

void updateIsTurn() {
	isTurn = !isTurn;
}

void updateRectangle(int _value) {
	plane.update(_value);

	if (!isTurn) {
		glutPostRedisplay();
	}
}

void keyDownInput(unsigned char key, int x, int y) {
	switch (key) {
	case Q:
		exit(0);
		break;
	case NUM_1:
		updateRectangle(-1);
		break;

	case NUM_2:
		updateRectangle(1);
		break;

	case SPACEBAR:
		updateIsTurn();
		break;
	case W:
		gWave = !gWave;
		glutPostRedisplay();
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Hello GL");

	glewExperimental = true;
	glewInit();

	init();
	glutIdleFunc(myIdle);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyDownInput);
	glutMainLoop();

	return 0;
}