#include <vgl.h>
#include <InitShader.h>
#include "MyCube.h"

#include <vec.h>
#include <mat.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_map>

using namespace std;

struct MyModelVertex
{
	vec4 position;
	vec4 color;
	vec3 normal;
	float verNum;
	vec3 phNormal;
};

unordered_map<int, vector<vec3>> phongNormal;

MyCube cube;

GLuint program;
GLuint prog_phong;

GLuint uMat;
GLuint uColor;
mat4 g_Mat = mat4(1.0f);

int winWidth = 500;
int winHeight = 500;

char fileName[255];
ifstream openFile;

vector<vec4> vertex;
vector<vec4> frag;

MyModelVertex* Vertices;

//vec4* pos;
//vec4* colors;
//vec3* normal;

int index = 0;
int numVert = 0;

GLuint vao;
GLuint buffer;

float maxX = -100000, maxY = -100000, maxZ = -100000;
float sumX = 0, sumY = 0, sumZ = 0;
float avgX = 0, avgY = 0, avgZ = 0;
float scaleAll = 1;

float turnX = 0.0f;
float turnY = 0.0f;
bool isRotate = false;
bool isY = true;
float isFlat = 0;


float shiness = 5.0;
vec4 mSpec = vec4(0.3, 0.3, 0.3, 1);

mat4 myLookAt(vec3 eye, vec3 at, vec3 up)
{
	// Implement your own look-at function
	mat4 V(1.0f);
	vec3 n = at - eye;
	n /= length(n);

	float a = dot(up, n);
	vec3 v = up - a * n;
	v /= length(v);

	vec3 w = cross(n, v);

	mat4 Rw(1.0f);

	Rw[0][0] = w.x;	Rw[0][1] = v.x; Rw[0][2] = -n.x;
	Rw[1][0] = w.y;	Rw[1][1] = v.y; Rw[1][2] = -n.y;
	Rw[2][0] = w.z;	Rw[2][1] = v.z; Rw[2][2] = -n.z;

	mat4 Rc(1.0f);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			Rc[i][j] = Rw[j][i];

	mat4 Tc = Translate(-eye.x, -eye.y, -eye.z);

	V = Rc * Tc;

	return V;
}

vec3 transVec3(vec4 a) {
	vec3 sub;

	sub.x = a.x;
	sub.y = a.y;
	sub.z = a.z;

	return sub;
}

vec3 computeNormal(vec4 p0, vec4 p1, vec4 p2)
{
	vec4 a4 = p1 - p0;         // p0 --> p1
	vec4 b4 = p2 - p0;         // p0 --> p2
	vec3 a = vec3(a4.x, a4.y, a4.z);
	vec3 b = vec3(b4.x, b4.y, b4.z);

	vec3 n = normalize((cross(a, b)));
	return n;
}

void setVertexNormal()
{
	for (int i = 0; i < index; i++)
	{	
		vec3 normal = (0, 0, 0);


		for (int j = 0; j < phongNormal[Vertices[i].verNum].size(); j++) {
			normal += phongNormal[Vertices[i].verNum][j];
		}

		normal = normalize(normal);
		Vertices[i].phNormal = normal;

	}
}



void triple(int a, int b, int c) {

	/*vec3 vA = transVec3(vertex[a]);
	vec3 vB = transVec3(vertex[b]);
	vec3 vC = transVec3(vertex[c]);

	vec3 nor = (vA + vB + vC) / 3;*/

	vec3 normal = computeNormal(vertex[a], vertex[b], vertex[c]);

	/*Vertices[index].position = vertex[a]; Vertices[index].color = vec4(1.0, 1.0, 1.0, 1); Vertices[index].normal = computeNormal(vertex[a], vertex[b], vertex[c]); index++;
	Vertices[index].position = vertex[b]; Vertices[index].color = vec4(1.0, 1.0, 1.0, 1); Vertices[index].normal = computeNormal(vertex[a], vertex[b], vertex[c]); index++;
	Vertices[index].position = vertex[c]; Vertices[index].color = vec4(1.0, 1.0, 1.0, 1); Vertices[index].normal = computeNormal(vertex[a], vertex[b], vertex[c]); index++;*/

	Vertices[index].verNum = a; Vertices[index].position = vertex[a]; Vertices[index].color = vec4(0.5, 0.5, 0.5, 1); Vertices[index].normal = normal; index++;
	Vertices[index].verNum = b; Vertices[index].position = vertex[b]; Vertices[index].color = vec4(0.5, 0.5, 0.5, 1); Vertices[index].normal = normal; index++;
	Vertices[index].verNum = c; Vertices[index].position = vertex[c]; Vertices[index].color = vec4(0.5, 0.5, 0.5, 1); Vertices[index].normal = normal; index++;

	phongNormal[a].push_back(normal);
	phongNormal[b].push_back(normal);
	phongNormal[c].push_back(normal);
}

void setPosition() {
	for (int i = 0; i < frag.size(); i++) {
		triple(frag[i].x, frag[i].y, frag[i].z);
	}
	setVertexNormal();
}

void calXYZ() {
	float s = vertex.size();
	for (int i = 1; i < s; i++) {
		if (vertex[i].x > maxX) maxX = vertex[i].x;
		if (vertex[i].y > maxY) maxY = vertex[i].y;
		if (vertex[i].z > maxZ) maxZ = vertex[i].z;

		sumX += vertex[i].x;
		sumY += vertex[i].y;
		sumZ += vertex[i].z;
	}

	avgX = sumX / (s-1);
	avgY = sumY / (s-1);
	avgZ = sumZ / (s-1);

	float scaleX = (1.0 - maxX) * 10 + 1;

	float scaleY = (1.0 - maxY) * 10 + 1;

	float scaleZ = (1.0 - maxZ) * 10 + 1;

	if (scaleX > scaleY) {
		if (scaleY > scaleZ)
			scaleAll = scaleZ;

		else
			scaleAll = scaleY;
	}

	else if (scaleX < scaleY) {
		if (scaleX < scaleZ)
			scaleAll = scaleX;

		else
			scaleAll = scaleZ;

	}
}

mat4 myOrtho(float l, float r, float b, float t, float zNear, float zFar)
{
	// Implement your own Ortho function
	mat4 V(1.0f);

	V[0][0] = 2 / (r - l);
	V[1][1] = 2 / (t - b);
	V[2][2] = 2 / (zFar - zNear);
	V[0][3] = -(r + l) / (r - l);
	V[1][3] = -(t + b) / (t - b);
	V[2][3] = (zNear) / (zFar - zNear);

	return V;
}

void saveArray(string line) {
	if (!(line[0] == 'f' || line[0] == 'v'))
		return;

	istringstream ss(line);
	string buffer;
	vector<string> bufArr;


	if (line[0] == 'v') {
		while (getline(ss, buffer, ' ')) {
			bufArr.push_back(buffer);
		}

		vec3 v;

		v.x = stof(bufArr[1]);
		v.y = stof(bufArr[2]);
		v.z = stof(bufArr[3]);

		vertex.push_back(v);
	}

	if (line[0] == 'f') {
		while (getline(ss, buffer, ' ')) {
			bufArr.push_back(buffer);
		}

		vec3 v;
		v.x = stoi(bufArr[1]);
		v.y = stoi(bufArr[2]);
		v.z = stoi(bufArr[3]);

		frag.push_back(v);
	}
}

void readFile() {


	while (1) {
		cout << "Input FIle Path: ";
		cin >> fileName;

		openFile.open(fileName);

		if (!openFile) {
			cout << "File not Found!\n";
			continue;
		}
		break;
	}

	string line;

	if (openFile.is_open()) {
		while (getline(openFile, line)) {
			saveArray(line);
		}
		openFile.close();
	}

}

mat4 myPerspective(float fovy, float aspectRatio, float zNear, float zFar)
{
	mat4 P(1.0f);

	float rad = fovy * 3.141592 / 180.0;

	float sz = 1 / zFar;
	float h = zFar * tan(rad / 2);

	float sy = 1 / h;
	float w = h * aspectRatio;
	float sx = 1 / w;

	mat4 S = Scale(sx, sy, sz);
	mat4 M(1.0f);

	float c = -zNear / zFar;
	M[2][2] = 1 / (c + 1);
	M[2][3] = -c / (c + 1);
	M[3][2] = -1;
	M[3][3] = 0;

	P = M * S;

	return P;
}


void myInit()
{
	cube.Init();
	//sphere.Init(20, 20);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyModelVertex) * numVert, Vertices, GL_STATIC_DRAW);

	program = InitShader("vshader.glsl", "fshader.glsl");
	prog_phong = InitShader("vphong.glsl", "fphong.glsl");
}


void keyboard(unsigned char ch, int x, int y)
{
	if (ch == ' ')
		isRotate = !isRotate;
	else if (ch == '1') {
		cout << "Using Vertex Normal" << endl;

		isFlat = 0;

	}
	else if (ch == '2') {
		cout << "Using Surface Normal" << endl;
			
		isFlat = 1;
	}
	else if (ch == '3') {
		cout << "Increasing Specular Effect" << endl;
		if (mSpec.x < 1.0) {
			mSpec.x += 0.1;
			mSpec.y += 0.1;
			mSpec.z += 0.1;
		}
	}
	else if (ch == '4') {
		cout << "Decreasing Specular Effect" << endl;

		if (mSpec.x > 0.2) {
			mSpec.x -= 0.1;
			mSpec.y -= 0.1;
			mSpec.z -= 0.1;
		}
	}
	else if (ch == '5') {
		cout << "Increasing Shiness" << endl;

		if(shiness < 50)
			shiness++;
	}
	else if (ch == '6') {
		cout << "Decreasing Shiness" << endl;

		if (shiness > 1)
			shiness--;
	}
}

void processMouse(int button, int state, int x, int y) {

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		isY = true;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		isY = false;
	}
}

void DrawAxis()
{
	glUseProgram(program);
	mat4 x_a = Translate(1, 0, 0) * Scale(2, 0.01, 0.01);
	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * x_a);
	glUniform4f(uColor, 1, 0, 0, 1);
	cube.Draw(program);

	mat4 y_a = Translate(0, 1., 0)  * Scale(0.01, 2, 0.01);
	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * y_a);
	glUniform4f(uColor, 0, 1, 0, 1);
	cube.Draw(program);

	mat4 z_a = Translate(0, 0, 1) * Scale(0.01, 0.01, 2);
	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * z_a);
	glUniform4f(uColor, 0, 0, 1, 1);
	cube.Draw(program);
}

void DrawGrid()
{
	glUseProgram(program);
	float n = 40;
	float w = 10;
	float h = 10;

	for (int i = 0; i < n; i++)
	{
		mat4 m = Translate(0, -0.5, -h + 2 * h / n * i) * Scale(w * 2, 0.02, 0.02);
		glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * m);
		glUniform4f(uColor, 1, 1, 1, 1);
		cube.Draw(program);
	}
	for (int i = 0; i < n; i++)
	{
		mat4 m = Translate(-w + 2 * w / n * i, -0.5, 0) * Scale(0.02, 0.02, h * 2);
		glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * m);
		glUniform4f(uColor, 1, 1, 1, 1);
		cube.Draw(program);
	}
}

float g_Time = 0;

void display()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	mat4 ViewMat = myLookAt(vec3(0, 0, 3), vec3(0, 0, 0), vec3(0, 1, 0));

	float aspect = winWidth / (float)winHeight;
	float h = 1;

	//mat4 ProjMat = myOrtho(-h*aspect,h*aspect,-h,h,0,100);
	mat4 ProjMat = myPerspective(40, aspect, 0.1, 100);

	g_Mat = ProjMat * ViewMat;

	glUseProgram(program);
	uMat = glGetUniformLocation(program, "uMat");
	uColor = glGetUniformLocation(program, "uColor");

	DrawAxis();
	//DrawGrid();

	glUseProgram(prog_phong);
	GLuint uProjMat = glGetUniformLocation(prog_phong, "uProjMat");
	GLuint uModelMat = glGetUniformLocation(prog_phong, "uModelMat");


	mat4 ModelMat = 1;

	if (isRotate) {
		if(isY){
			turnY += 0.016;
		}
		else {
			turnX += 0.016;
		}
	}
	ModelMat *= RotateY(turnY * 90) * RotateX(turnX * 90);

	mat4 TransMat = Translate(-avgX, -avgY, -avgZ);

	mat4 ScaleMat = Scale(scaleAll, scaleAll, scaleAll);

;	glUniformMatrix4fv(uProjMat, 1, GL_TRUE, ProjMat);
	glUniformMatrix4fv(uModelMat, 1, GL_TRUE, ViewMat * ScaleMat * ModelMat * TransMat);

	// 1. define lights : Position, Color(Intensity)
	vec4 lpos = vec4(2, 2, 0, 1);	// in World Coord.
	vec4 lcol = vec4(1, 1, 1, 1);	// white 

	// 2. material properties (phong coeff.)
	vec4 mAmb = vec4(0.2, 0.2, 0.2, 1);
	vec4 mDif = vec4(0.6, 0.3, 0.3, 1);
	
	

	GLuint uLPos = glGetUniformLocation(prog_phong, "uLPos");
	GLuint uLCol = glGetUniformLocation(prog_phong, "uLCol");
	GLuint uAmb = glGetUniformLocation(prog_phong, "uAmb");
	GLuint uDif = glGetUniformLocation(prog_phong, "uDif");
	GLuint uSpec = glGetUniformLocation(prog_phong, "uSpec");
	GLuint uShiness = glGetUniformLocation(prog_phong, "uShiness");
	GLuint uIsFlat = glGetUniformLocation(prog_phong, "uIsFlat");

	glUniform4f(uLPos, lpos[0], lpos[1], lpos[2], lpos[3]);
	glUniform4f(uLCol, lcol[0], lcol[1], lcol[2], lcol[3]);
	glUniform4f(uAmb, mAmb[0], mAmb[1], mAmb[2], mAmb[3]);
	glUniform4f(uDif, mDif[0], mDif[1], mDif[2], mDif[3]);
	glUniform4f(uSpec, mSpec[0], mSpec[1], mSpec[2], mSpec[3]);
	glUniform1f(uShiness, shiness);
	glUniform1f(uIsFlat, isFlat);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	GLuint vPosition = glGetAttribLocation(prog_phong, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(MyModelVertex), BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(prog_phong, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, sizeof(MyModelVertex), BUFFER_OFFSET(sizeof(vec4)));

	GLuint vNormal = glGetAttribLocation(prog_phong, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_TRUE, sizeof(MyModelVertex), BUFFER_OFFSET(sizeof(vec4) + sizeof(vec4)));

	GLuint vPhNormal = glGetAttribLocation(prog_phong, "vPhNormal");
	glEnableVertexAttribArray(vPhNormal);
	glVertexAttribPointer(vPhNormal, 3, GL_FLOAT, GL_TRUE, sizeof(MyModelVertex), BUFFER_OFFSET(sizeof(vec4) + sizeof(vec4) + sizeof(vec3) + sizeof(GL_FLOAT)));

	glDrawArrays(GL_TRIANGLES, 0, numVert);

	//sphere.Draw(prog_phong);

	glutSwapBuffers();
}


void idle()
{

	Sleep(16);					// for vSync
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glutPostRedisplay();
}


int main(int argc, char** argv)
{	
	vec3 tmp;
	vertex.push_back(tmp);

	readFile();
	numVert = frag.size() * 3;

	Vertices = (MyModelVertex *)malloc(sizeof(MyModelVertex) * numVert);

	//pos = (vec4*)malloc(sizeof(vec4) * numVert);
	//colors = (vec4*)malloc(sizeof(vec4) * numVert);

	setPosition();
	calXYZ();

	//	for (int i = 0; i < numVert; i++) {
	//	if (i % 3 == 0)
	//		cout << endl;
	//	cout << pos[i] << endl;
	//}

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(winWidth, winHeight);

	glutCreateWindow("OpenGL");

	glewExperimental = true;
	glewInit();

	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));

	myInit();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(processMouse);
	glutMainLoop();
	

	return 0;
}
