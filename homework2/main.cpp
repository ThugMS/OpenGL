#include <vgl.h>
#include <InitShader.h>
#include <mat.h>
#include <iostream>
#include "MyCube.h"
#include "MyPyramid.h"
#include "MyTarget.h"

#define PI 3.141592

using namespace std;

MyCube cube;
MyPyramid pyramid;
MyTarget target(&cube);

GLuint program;
GLuint uMat;

mat4 CTM;
vec3 prevPosition = (0, 0, 0);

bool bPlay = false;
bool bChasingTarget = false;
bool bDrawTarget = false;

float ang1 = 0;
float ang2 = 0;
float ang3 = 0;

float targetAng1 = 0;
float targetAng2 = 0;
float targetAng3 = 0;

int targetArrow1 = 0;
int targetArrow2 = 0;
int targetArrow3 = 0;

bool nonChange1 = false;
bool nonChange2 = false;
bool nonChange3 = false;

int calNum = 0;

void myInit()
{
	cube.Init();
	pyramid.Init();

	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);
}

float g_time = 0;

void drawRobotArm(float ang1, float ang2, float ang3)
{
	mat4 temp = CTM;
	mat4 M(1.0);

#pragma region Base
	M = Translate(0, 0, 0.075) * Scale(0.3, 0.2, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	pyramid.Draw(program);

	M = Translate(0, 0, -0.075) * Scale(0.3, 0.2, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	pyramid.Draw(program);

	M = Translate(0, 0.00, -0.1) * Scale(0.05, 0.05, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	M = Translate(0, 0.00, 0.1) * Scale(0.05, 0.05, -0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);
#pragma endregion
	
#pragma region Lower Arm
	CTM *= RotateZ(-ang1) * Translate(0, 0.15, 0);
	M = Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);
#pragma endregion


#pragma region Upper Arm
	CTM *= Translate(0, 0.15, 0) * RotateZ(-ang2) * Translate(0, 0.2, 0);
	M = Translate(0, 0, -0.075) * Scale(0.1, 0.5, 0.03);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	M = Translate(0, 0, 0.075) * Scale(0.1, 0.5, 0.03);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	M = Translate(0, -0.2, 0.1) * Scale(0.05, 0.05, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	M = Translate(0, -0.2, -0.1) * Scale(0.05, 0.05, -0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

#pragma endregion
#pragma region Hand
	CTM *= Translate(0, 0.2, 0) * RotateZ(-ang3);
	M = Scale(0.15, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	M = Translate(0, 0, 0.1) * Scale(0.05, 0.05, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);

	M = Translate(0, 0, -0.1) * Scale(0.05, 0.05, -0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM * M);
	cube.Draw(program);
#pragma endregion
	CTM = temp;
}

float calculateDis(vec3 a, vec3 b)
{	
	float distance;

	distance = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
	return distance;
}

float degreeToRadian(float angle) {
	float radianAngle;

	radianAngle = angle * PI / 180;
	return radianAngle;
}

int calculatieDirect(vec3 targetPos, float upper_length)
{
	float change = 0.05;
	vec3 basePos(0, 0, 0);

	vec3 minus = basePos + vec3(sin(degreeToRadian(ang1 - change)) *upper_length, cos(degreeToRadian(ang1 - change)) * upper_length, 0);
	vec3 plus = basePos + vec3(sin(degreeToRadian(ang1 + change)) * upper_length, cos(degreeToRadian(ang1 + change)) * upper_length, 0);

	double plusDis = calculateDis(minus, targetPos);
	double minusDis = calculateDis(plus, targetPos);

	if (plusDis > minusDis)
		return 1;

	return -1;
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	GLuint uColor = glGetUniformLocation(program, "uColor");
	glUniform4f(uColor, -1, -1, -1, -1);


	uMat = glGetUniformLocation(program, "uMat");
	CTM = Translate(0, -0.4, 0) * RotateY(g_time * 30);

	drawRobotArm(ang1, ang2, ang3);

	glUniform4f(uColor, 1, 0, 0, 1);
	if (bDrawTarget == true)
		target.Draw(program, CTM, g_time);

	glutSwapBuffers();
}

float lawOfCos(float a, double b, float c) {
	float angle;

	angle = acos((a * a + b * b - c * c) / (2 * a * b));
	angle = angle * 180 / PI;

	return angle;
}

float getCosAngle(vec3 centerJoint, vec3 jointTarget) {

	float angleTarget = acos((centerJoint.x * jointTarget.x + centerJoint.y * jointTarget.y) / (sqrt(centerJoint.x * centerJoint.x + centerJoint.y * centerJoint.y) * sqrt(jointTarget.x * jointTarget.x + jointTarget.y * jointTarget.y))) * 180 / PI;

	return angleTarget;
}

void calculateAngle(vec3 joint, vec3 upper_offset, float upperArmLen, vec3 targetPos, float lowerArmLen, float handLen, float angChange, vec3 centerPos) {
	float sub = targetAng1;
	

	while (1)
	{
		joint = upper_offset + vec3(sin(degreeToRadian(targetAng1)) * upperArmLen, cos(degreeToRadian(targetAng1)) * upperArmLen, 0);

		vec3 centerJoint = centerPos - joint;
		vec3 jointTarget = targetPos - joint;
		double targetPosDis = calculateDis(joint, targetPos);

		if (targetPosDis >= lowerArmLen + handLen || targetPosDis + handLen <= lowerArmLen)
			targetAng1 += angChange;

		targetAng2 = lawOfCos(lowerArmLen, targetPosDis, handLen); 
		if (isnan(targetAng2))
			continue;

		

		float angleTarget = getCosAngle(centerJoint, jointTarget);

		targetAng2 = 180 - (targetAng2 + angleTarget);
		if (angChange < 0)
			targetAng2 = -targetAng2;
		targetAng3 = 180 - lawOfCos(lowerArmLen, handLen, targetPosDis);
		if (angChange < 0)
			targetAng3 = -targetAng3;
		if (isnan(targetAng3))
			continue;


		break;
	}

	targetArrow1 = (targetAng1 - ang1 > 0) - (targetAng1 - ang1 < 0);
	targetArrow2 = (targetAng2 - ang2 > 0) - (targetAng2 - ang2 < 0);
	targetArrow3 = (targetAng3 - ang3 > 0) - (targetAng3 - ang3 < 0);

	if (abs(targetAng1 - ang1) > 180)
		targetArrow1 *= -1;
	if (abs(targetAng2 - ang2) > 180)
		targetArrow2 *= -1;
	if (abs(targetAng3 - ang3) > 180)
		targetArrow3 *= -1;

	if (abs(targetAng1 - ang1) < 5)
		nonChange1 = true;
	else
		nonChange1 = false;

	if (abs(targetAng2 - ang2) < 3)
		nonChange2 = true;
	else
		nonChange2 = false;

	if (abs(targetAng3 - ang3) < 1)
		nonChange3 = true;
	else
		nonChange3 = false;


	targetArrow1 *= 5;
	targetArrow2 *= 3;
	targetArrow3 *= 1;
}

float checkCorrectAngle(float a) {
	if (a >= 180) {
		a -= 360;
	}
	else if (a <= -180) {
		a += 360;
	}
	return a;
}

bool checkingDestination(float ang, float target) {
	if (abs(ang - target) < 5)
		return true;
	return false;
}

void computeAngle()
{
	vec3 centerPos(0, 0, 0);
	vec3 targetPos = target.currentPos; //타겟 위치
	vec3 joint; //관절

	float upperArmLen = 0.3;
	float lowerArmLen = 0.4;
	float handLen = 0.2;

	float changeAngle = 1.5;
	vec3 upperChange(0, 0, 0);
	changeAngle = calculatieDirect(targetPos, upperArmLen) * changeAngle;


	targetAng1 = ang1;
	targetAng2 = ang2;
	targetAng3 = ang3;

	calculateAngle(joint, upperChange, upperArmLen, targetPos, lowerArmLen, handLen, changeAngle, centerPos);

	if (!nonChange1) {
		ang1 += targetArrow1;
		ang1 = checkCorrectAngle(ang1);
	}
	if (!nonChange2) {
		ang2 += targetArrow2;
		ang2 = checkCorrectAngle(ang2);
		cout << targetAng2 << endl;
		cout << ang2 << endl << endl;
		nonChange2 = checkingDestination(ang2, targetAng2);
	}
	
	if (!nonChange3) {
		ang3 += targetArrow3;
		ang3 = checkCorrectAngle(ang3);
		nonChange3 = checkingDestination(ang3, targetAng3);
	}
}




void myIdle()
{
	if (bPlay)
	{
		g_time += 1 / 60.0f;
		Sleep(1 / 60.0f * 1000);

		if (bChasingTarget == false)
		{
			ang1 = 45 * sin(g_time * 3.141592);
			ang2 = 60 * sin(g_time * 2 * 3.141592);
			ang3 = 30 * sin(g_time * 3.141592);
		}
		else
			computeAngle();

		glutPostRedisplay();
	}
}

void myKeyboard(unsigned char c, int x, int y)
{

	switch (c)
	{
	case '1':
		bChasingTarget = !bChasingTarget;
		break;
	case '2':
		bDrawTarget = !bDrawTarget;
		break;
	case '3':
		target.toggleRandom();
		break;
	case ' ':
		bPlay = !bPlay;
		break;
	default:
		break;
	}
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Simple Robot Arm");

	glewExperimental = true;
	glewInit();

	myInit();
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutIdleFunc(myIdle);

	glutMainLoop();

	return 0;
}