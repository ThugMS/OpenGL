
#define _CRT_SECURE_NO_WARNINGS

#include <vgl.h>
#include <InitShader.h>
#include <iostream>
#include <vector>
#include "MyCube.h"
#include "MyUtil.h"

#include <vec.h>
#include <mat.h>

#define MAZE_FILE	"maze.txt"

using namespace std;

MyCube cube;
GLuint program;

mat4 g_Mat = mat4(1.0f);
GLuint uMat;
GLuint uColor;

float wWidth = 1000;
float wHeight = 500;

vec3 cameraPos = vec3(0, 0, 0);
vec3 viewDirection = vec3(0, 0, -1);
vec3 goalPos = vec3(0, 0, 0);

int MazeSize;
char maze[255][255] = { 0 };

int dirX[4] = {0,0,-1,1};
int dirY[4] = {-1,1,0,0};

float cameraSpeed = 0.1;

float g_time = 0;

double turnAngle = 0 + ((2*3.141592653589) / 4) * 3;

bool isLeftTurn = false;
bool isRightTurn = false;
bool isNavigate = false;

int cnt = 0;
int maxCnt = 0;
int naviArrow = 0;

int turnCnt = 0;

typedef struct node {
	int h = 0;
	int f = 0;
	int g = 0;
	pair<int, int> parent;
	pair<int, int> index;
	pair<int, int> child;
};

vector<node> closeNode;
vector<node> openNode;
vector<node> answerNode;

vector<vec3> navigateArrow;

pair<int, int> goalIndex;
pair<int, int>startIndex;

inline vec3 getPositionFromIndex(int i, int j)
{
	float unit = 1;
	vec3 leftTopPosition = vec3(-MazeSize / 2.0 + unit / 2, 0, -MazeSize / 2.0 + unit / 2);
	vec3 xDir = vec3(1, 0, 0);
	vec3 zDir = vec3(0, 0, 1);
	return leftTopPosition + i * xDir + j * zDir;
}

bool CheckCollider(vec3 pos) {
	float unit = 1;
	vec3 leftTopPosition = vec3(MazeSize / 2.0 , 0, MazeSize / 2.0);

	pos += leftTopPosition;

	if (maze[(int)(pos.x)][(int)(pos.z)] == '*') {

		maze[(int)(pos.x)][(int)(pos.z)] = '|';
		return true;
	}
	return false;
}

void LoadMaze()
{
	FILE* file = fopen(MAZE_FILE, "r");
	char buf[255];
	fgets(buf, 255, file);
	sscanf(buf, "%d", &MazeSize);
	for (int j = 0; j < MazeSize; j++)
	{
		fgets(buf, 255, file);
		for (int i = 0; i < MazeSize; i++)
		{
			maze[i][j] = buf[i];
			if (maze[i][j] == 'C')				// Setup Camera Position
				cameraPos = getPositionFromIndex(i, j);
			if (maze[i][j] == 'G') {				// Setup Goal Position
				goalPos = getPositionFromIndex(i, j);
				goalIndex.first = j;
				goalIndex.second = i;
			}
		}
	}
	fclose(file);
}

float RadianToDegree(float angle) {
	return angle / 3.141592 * 180;
}

float DegreeToRadian(float angle) {
	return angle / 180 * 3.141592;
}

float GetAngle(vec3 v1, vec3 v2) {
	v1 = normalize(v1);
	v2 = normalize(v2);

	int direction = 1;
	if (cross(v1, v2).y > 0)
		direction = -1;

	return direction * RadianToDegree(acos(dot(v1, v2)));
}

void DrawMaze(bool check)
{
	for (int j = 0; j < MazeSize; j++)
		for (int i = 0; i < MazeSize; i++) {


			if (maze[i][j] == '*') 
			{
				vec3 color = vec3(i / (float)MazeSize, j / (float)MazeSize, 1);
				mat4 ModelMat = Translate(getPositionFromIndex(i, j));
				glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * ModelMat);
				glUniform4f(uColor, color.x, color.y, color.z, 1);
				cube.Draw(program);
			}

			if (maze[i][j] == '|') {
				vec3 color = vec3(255, 0, 0);
				mat4 ModelMat = Translate(getPositionFromIndex(i, j));
				glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * ModelMat);
				glUniform4f(uColor, color.x, color.y, color.z, 1);
				cube.Draw(program);
				if(check)
					maze[i][j] = '*';
			}
		}
}

void myInit()
{
	LoadMaze();
	cube.Init();
	program = InitShader("vshader.glsl", "fshader.glsl");
}

void DrawGrid()
{
	float n = 40;
	float w = MazeSize;
	float h = MazeSize;

	for (int i = 0; i < n; i++)
	{
		mat4 m = Translate(0, -0.5, -h / 2 + h / n * i) * Scale(w, 0.02, 0.02);
		glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * m);
		glUniform4f(uColor, 1, 1, 1, 1);
		cube.Draw(program);
	}
	for (int i = 0; i < n; i++)
	{
		mat4 m = Translate(-w / 2 + w / n * i, -0.5, 0) * Scale(0.02, 0.02, h);
		glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * m);
		glUniform4f(uColor, 1, 1, 1, 1);
		cube.Draw(program);
	}
}

void SetTurnAngle(int a) {
	switch(a) {
	case 1:
		turnAngle = 0 + ((2 * 3.141592653589) / 4) * 3;
		break;

	case 2:
		turnAngle = 0 + ((2 * 3.141592653589) / 4) * 1;
		break;

	case 3:
		turnAngle = 0 + ((2 * 3.141592653589) / 4) * 2;
		break;

	case 4:
		turnAngle = 0 + ((2 * 3.141592653589) / 4) * 4;
		break;
	}
}

void DrawRoad() {
	float w = 1;

	for (int i = 0; i < answerNode.size(); i++) {
		vec3 pos = getPositionFromIndex(answerNode[i].index.second, answerNode[i].index.first);

		pair<int, int> parent = answerNode[i].parent;
		pair<int, int> cur = answerNode[i].index;
		pair<int, int> child = answerNode[i].child;
		mat4 m;
		
		if (answerNode.size() - 1 == i) {
			if(cur.first == child.first)
				m = Translate(pos.x, -0.3, pos.z) * Scale(w, 0.1, 0.1);
			else
				m = Translate(pos.x, -0.3, pos.z) * Scale(0.1, 0.1, w);

			glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * m);
			glUniform4f(uColor, 1, 0, 0, 1);
			cube.Draw(program);

			continue;
		}

		if ((parent.first == cur.first && parent.second > cur.second && child.second == cur.second && child.first < cur.first) || (child.first == cur.first && child.second > cur.second && parent.second == cur.second && parent.first < cur.first)) { // ┕ 모양
			m = Translate(pos.x, -0.3, pos.z - 0.25) * Scale(0.1, 0.1, w/2);
			glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * m);
			glUniform4f(uColor, 1, 0, 0, 1);
			cube.Draw(program);

			m = Translate(pos.x + 0.25, -0.3, pos.z) * Scale(w/2, 0.1, 0.1);
		}
		else if ((parent.first == cur.first && parent.second > cur.second && child.second == cur.second && child.first > cur.first) || (child.first == cur.first && child.second > cur.second && parent.second == cur.second && parent.first > cur.first)) { // ┎ 모양
			m = Translate(pos.x, -0.3, pos.z + 0.25) * Scale(0.1, 0.1, w / 2);
			glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * m);
			glUniform4f(uColor, 1, 0, 0, 1);
			cube.Draw(program);

			m = Translate(pos.x + 0.25, -0.3, pos.z) * Scale(w / 2, 0.1, 0.1);
		}
		else if ((parent.first == cur.first && parent.second < cur.second && child.second == cur.second && child.first > cur.first) || (child.first == cur.first && child.second < cur.second && parent.second == cur.second && parent.first > cur.first)) { // ┐ 모양
			m = Translate(pos.x, -0.3, pos.z + 0.25) * Scale(0.1, 0.1, w / 2);
			glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * m);
			glUniform4f(uColor, 1, 0, 0, 1);
			cube.Draw(program);

			m = Translate(pos.x - 0.25, -0.3, pos.z) * Scale(w / 2, 0.1, 0.1);
		}
		else if ((parent.first == cur.first && parent.second < cur.second && child.second == cur.second && child.first < cur.first) || (child.first == cur.first && child.second < cur.second && parent.second == cur.second && parent.first < cur.first)) { // ┛ 모양
			m = Translate(pos.x, -0.3, pos.z - 0.25) * Scale(0.1, 0.1, w / 2);
			glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * m);
			glUniform4f(uColor, 1, 0, 0, 1);
			cube.Draw(program);

			m = Translate(pos.x - 0.25, -0.3, pos.z) * Scale(w / 2, 0.1, 0.1);
		}
		else if (parent.first != cur.first && parent.second == cur.second) { //세로줄
			m = Translate(pos.x, -0.3, pos.z) * Scale(0.1, 0.1, w);
		}
		else if (parent.second != cur.second && parent.first == parent.first) { //가로줄
			m = Translate(pos.x, -0.3, pos.z) * Scale(w, 0.1, 0.1);
		}

		glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * m);
		glUniform4f(uColor, 1,0, 0, 1);
		cube.Draw(program);
	}
}

void drawCamera()
{
	float cameraSize = 0.5;

	mat4 ModelMat = Translate(cameraPos) * RotateY(GetAngle(viewDirection, vec3(-1, 0, 0))) * Scale(vec3(cameraSize));

	if (isLeftTurn) {
		turnAngle -= 0.1;
	}

	if (isRightTurn) {
		turnAngle += 0.1;
	}
		
	
	glUseProgram(program);
	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * ModelMat);
	glUniform4f(uColor, 0, 1, 0, 1);
	cube.Draw(program);

	ModelMat = Translate(cameraPos + viewDirection * cameraSize / 2) * Scale(vec3(cameraSize / 2)) * RotateY(GetAngle(viewDirection,vec3(-1,0,0)));

	glUseProgram(program);
	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * ModelMat);
	glUniform4f(uColor, 0, 1, 0, 1);
	cube.Draw(program);

	isLeftTurn = false;
	isRightTurn = false;
}

void drawGoal()
{
	glUseProgram(program);
	float GoalSize = 0.7;

	mat4 ModelMat = Translate(goalPos) * RotateY(g_time * 3) * Scale(vec3(GoalSize));
	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * ModelMat);
	glUniform4f(uColor, 0, 0, 0, 0);
	cube.Draw(program);

	ModelMat = Translate(goalPos) * RotateY(g_time * 3 + 45) * Scale(vec3(GoalSize));
	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * ModelMat);
	glUniform4f(uColor, 0, 0, 0, 0);
	cube.Draw(program);
}


void drawScene(bool bDrawCamera = true)
{
	glUseProgram(program);
	uMat = glGetUniformLocation(program, "uMat");
	uColor = glGetUniformLocation(program, "uColor");

	DrawGrid();
	DrawMaze(bDrawCamera);
	drawGoal();
	DrawRoad();

	if (bDrawCamera)
		drawCamera();
}

void display()
{
	glEnable(GL_DEPTH_TEST);

	float vWidth = wWidth / 2;
	float vHeight = wHeight;

	// LEFT SCREEN : View From Camera (Perspective Projection)
	glViewport(0, 0, vWidth, vHeight);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	float h = 4;
	float aspectRatio = vWidth / vHeight;
	float w = aspectRatio * h;
	mat4 ViewMat = myLookAt(cameraPos, cameraPos + viewDirection, vec3(0, 1, 0));
	mat4 ProjMat = myPerspective(45, aspectRatio, 0.01, 20);

	g_Mat = ProjMat * ViewMat;
	drawScene(false);							// drawing scene except the camera


	// RIGHT SCREEN : View from above (Orthographic parallel projection)
	glViewport(vWidth, 0, vWidth, vHeight);
	h = MazeSize;
	w = aspectRatio * h;
	ViewMat = myLookAt(vec3(0, 5, 0), vec3(0, 0, 0), vec3(0, 0, -1));
	ProjMat = myOrtho(-w / 2, w / 2, -h / 2, h / 2, 0, 20);

	g_Mat = ProjMat * ViewMat;
	drawScene(true);


	glutSwapBuffers();
}

pair<int, int> GetIndex() {
	vec3 pos = vec3(cameraPos.x + MazeSize / 2.0, 0, cameraPos.z + MazeSize / 2.0);

	pair<int, int> index;
	index.second = pos.x;
	index.first = pos.z;

	//cout << index.first << " " << index.second << endl;

	return index;
}

void FindOpenNode(pair<int,int> currentIndex) {
	for (int i = 0; i < 4; i++) {
		pair<int, int> sub;

		sub.first = currentIndex.first + dirY[i];
		sub.second = currentIndex.second + dirX[i];

		bool isCloseNode = false;
		for (int i = 0; i < closeNode.size(); i++) {
			if (sub == closeNode[i].index)
				isCloseNode = true;
		}

		if (isCloseNode)
			continue;

		if (maze[sub.second][sub.first] == ' ' || maze[sub.second][sub.first] == 'G') {
			node child;

			child.parent = currentIndex;

			int sy = abs(startIndex.first - sub.first);
			int sx = abs(startIndex.second - sub.second);

			child.g = sx + sy;

			int gy = abs(goalIndex.first - sub.first);
			int gx = abs(goalIndex.second - sub.second);

			child.h = gx + gy;

			child.f = child.g + child.h;
			child.index = sub;

			bool isClear = true;

			for (int j = 0; j < openNode.size(); j++) {
				if (openNode[j].index == child.index) {
					if (openNode[j].g > child.g) {
						openNode.erase(openNode.begin() + j);
					}
					else {
						isClear = false;
					}
				}
			}

			if (isClear) {
				openNode.push_back(child);
			}
		}
	}
}

pair<int,int> SetCloseNode() {
	pair<int, int> closeIndex;

	if (openNode.empty())
		return closeIndex;
	node index = openNode[0];
	int indexNum = 0;
	
	for (int i = 1; i < openNode.size(); i++) {
		if (index.f > openNode[i].f) {
			index = openNode[i];
			indexNum = i;
		}
	}

	openNode.erase(openNode.begin()+indexNum);

	closeIndex = index.index;
	closeNode.push_back(index);

	return closeIndex;
}

void Astar() {
	openNode.clear();
	closeNode.clear();
	answerNode.clear();

	startIndex = GetIndex();
	pair<int, int> currentIndex = startIndex;
	
	node first;

	first.index = currentIndex;
	closeNode.push_back(first);
	
	int fcnt = 0;
	int scnt = 0;

	while (1) {
		FindOpenNode(currentIndex); 
		currentIndex = SetCloseNode();

		if (currentIndex == goalIndex) {
			break;
		}
		if ((GetAsyncKeyState('O') & 0x8000) == 0x8000) {		// if "S" key is pressed	: Go Backward
			break;
		}

	}
	
	pair<int, int> parentIndex;
	parentIndex = closeNode[closeNode.size() - 1].parent;
	answerNode.push_back(closeNode[closeNode.size() - 1]);

	for (int i = closeNode.size() - 1; i >= 0; i--) {
		
		if (parentIndex == startIndex) {
			closeNode[i].child = answerNode[answerNode.size()-1].index; 
		}

		if (parentIndex == closeNode[i].index) {
			answerNode.push_back(closeNode[i]);
			parentIndex = closeNode[i].parent;
		}

		
	}

	for (int i = 1; i < answerNode.size() - 1; i++) {
		answerNode[i].child = answerNode[i - 1].index;
	}

	/*for (int i = 1; i < answerNode.size() - 1; i++) {
		cout << answerNode[i].index.first << " " << answerNode[i].index.second << endl;
	}*/

	cout << "Find" << endl;
}

void Navigate() {
	if (answerNode.empty())
		return;

	navigateArrow.clear();
	isNavigate = true;

	int angle = 0;

	for (int i = answerNode.size() - 1; i > 0; i--) {
		vec3 start = getPositionFromIndex(answerNode[i].index.second, answerNode[i].index.first);
		vec3 end = getPositionFromIndex(answerNode[i - 1].index.second, answerNode[i - 1].index.first);

		//cout << start << endl << end << endl;

		vec3 arrow = normalize(end - start);
		navigateArrow.push_back(arrow);
	}
}

int SetNaviArrow(vec3 arrow) {
	if (arrow.z < 0)
		return 1;

	if (arrow.z > 0)
		return 2;

	if (arrow.x < 0)
		return 3;

	if (arrow.x > 0) {
		return 4;
	}
}

void CheckTurnDirection(int a, int b) {
	double angle = (2 * 3.141592653589 / 4) * 0.1;

	if (a == 1) {
		if (b == 3) {
			turnAngle -=  angle;
		}
		else {
			turnAngle += angle;
		}
	}

	else if (a == 2) {
		if (b == 3) {
			turnAngle += angle;
		}
		else {
			turnAngle -= angle;
		}
	}

	else if (a == 3) {
		if (b == 1) {
			turnAngle += angle;
		}
		else {
			turnAngle -= angle;
		}
	}

	else if (a == 4) {
		if (b == 1) {
			turnAngle -= angle;
		}
		else {
			turnAngle += angle;
		}
	}
}

void idle()
{
	g_time += 1;

	if (isNavigate) {
		maxCnt = navigateArrow.size() * 10;

		if (cnt == 0) {
			cameraPos = getPositionFromIndex(answerNode[answerNode.size() - 1].index.second, answerNode[answerNode.size() - 1].index.first);
			viewDirection = navigateArrow[0];
			naviArrow = SetNaviArrow(navigateArrow[0]);
			SetTurnAngle(SetNaviArrow(navigateArrow[0]));
		}

		if (naviArrow != SetNaviArrow(navigateArrow[cnt / 10])) {
			CheckTurnDirection(naviArrow, SetNaviArrow(navigateArrow[cnt / 10]));

			//cout << cnt  << " : " << turnAngle << endl;
			viewDirection.x = cos(turnAngle);
			viewDirection.z = sin(turnAngle);

			turnCnt++;

			if (turnCnt > 9) {
				turnCnt = 0;
				naviArrow = SetNaviArrow(navigateArrow[cnt / 10]);
			}
		}

		else {
			cameraPos += navigateArrow[cnt / 10] * cameraSpeed;
			cnt++;

			if (cnt >= maxCnt) {
				cnt = 0;
				isNavigate = false;
				//cout << maxCnt << " end" << endl;
			}
		}
		
	}
	else {
		if ((GetAsyncKeyState('A') & 0x8000) == 0x8000) {		// if "A" key is pressed	: Go Left
			isLeftTurn = true;

			viewDirection.x = cos(turnAngle);
			viewDirection.z = sin(turnAngle);


			/*cameraPos += cameraSpeed * vec3(-1, 0, 0);
			if(CheckCollider(cameraPos + cameraSpeed * vec3(-1,0,0) * 2))
				cameraPos -= cameraSpeed * vec3(-1, 0, 0);*/
		}
		if ((GetAsyncKeyState('D') & 0x8000) == 0x8000) {		// if "D" key is pressed	: Go Right
			isRightTurn = true;

			viewDirection.x = cos(turnAngle);
			viewDirection.z = sin(turnAngle);

			/*cameraPos += cameraSpeed * vec3(1, 0, 0);
			if (CheckCollider(cameraPos + cameraSpeed * vec3(1, 0, 0)))
				cameraPos -= cameraSpeed * vec3(1, 0, 0);*/
		}
		if ((GetAsyncKeyState('W') & 0x8000) == 0x8000) {		// if "W" key is pressed	: Go Forward

			cameraPos += cameraSpeed * viewDirection;

			if (CheckCollider(cameraPos + cameraSpeed * viewDirection))
				cameraPos -= cameraSpeed * viewDirection;
		}
		if ((GetAsyncKeyState('S') & 0x8000) == 0x8000) {		// if "S" key is pressed	: Go Backward
			cameraPos += cameraSpeed * viewDirection * -1;
			if (CheckCollider(cameraPos + cameraSpeed * viewDirection * -1))
				cameraPos -= cameraSpeed * viewDirection * -1;
		}
	}
		
	if ((GetAsyncKeyState(' ') & 0x8000) == 0x8000) {		// if "S" key is pressed	: Go Backward
		Navigate();
	}
	if ((GetAsyncKeyState('Q') & 0x8000) == 0x8000) {		// if "S" key is pressed	: Go Backward
		Astar();
	}

	


	//cout << cameraPos << endl;

	Sleep(16);											// for vSync
	glutPostRedisplay();
}

void reshape(int wx, int wy)
{
	printf("%d %d \n", wx, wy);
	wWidth = wx;
	wHeight = wy;
	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(wWidth, wHeight);

	glutCreateWindow("Homework3 (Maze Navigator)");

	glewExperimental = true;
	glewInit();

	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));

	myInit();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
}