#include <vgl.h>
#include <InitShader.h>
#include <mat.h>
#include "MyCube.h"
#include "MyPyramid.h"

MyCube cube;
MyPyramid pyramid;

GLuint program;
GLuint uMat;

mat4 g_Mat = mat4(1.0);

float g_Time = 0;

void myInit()
{
	cube.Init();
	pyramid.Init();

	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);
}



void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	GLuint uMat = glGetUniformLocation(program, "uMat");
	
	mat4 CTM(1.0);
	mat4 M(1.0);
 // 가장 오른쪽의 값부터 적용됨.
	//M = Translate(0.2, 0.4, 0) *
	//	RotateZ(90 * g_Time) *
	//	RotateY(30) *
	//	Scale(2, 0.5, 1);  //Scale -> Rotate -> Translate

	CTM = RotateY(30 * g_Time);
	//TRUE의 의미는 전치행렬을 해서  준다는 의미임.
	//M[i][j];  //row major  임. 

	
	M = Scale(0.5, 1, 0.2);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	pyramid.Draw(program);

	CTM *= Translate(0, 0.25, 0.25) * RotateZ(180.0 * g_Time);
	
	M = Scale(0.15, 0.15, 0.15);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	M = Scale(0.8, 0.1, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	M = RotateZ(90)*Scale(0.8, 0.1, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	glutSwapBuffers();
}

void myIdle()
{
	g_Time += 0.016f;
	Sleep(16);
	glutPostRedisplay();
}



int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutCreateWindow("Cube and Pyramid");

	glewExperimental = true;
	glewInit();

	myInit();
	glutDisplayFunc(myDisplay);
	glutIdleFunc(myIdle);

	glutMainLoop();

	return 0;
}