#include <vgl.h>
#include <InitShader.h>
#include <vec.h>
#include "MyColorCube.h"

float gTime = 0.0f;
GLuint prog;
MyColorCube cube;

void init()
{
	cube.init();
	prog = InitShader("1005_vShader.glsl", "1005_fShader.glsl");
	glUseProgram(prog);
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	cube.draw(prog, gTime);
	glFlush();
}

void myIdle()
{
	gTime += 0.001f;
	glutPostRedisplay();
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
	glutMainLoop();

	return 0;
}