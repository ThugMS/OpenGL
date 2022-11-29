//#define _CRT_SECURE_NO_WARNINGS
//
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <vector>
//#include <vgl.h>
//#include <InitShader.h>
//#include <MyCube.h>
//
//#include <vec.h>
//#include <mat.h>
//#include <stdio.h>
//
//using namespace std;
//
//
//GLuint program;
//GLuint prog_phong;
//
//GLuint uMat;
//GLuint uColor;
//mat4 g_Mat = mat4(1.0f);
//
//MyCube cube;
//int winWidth = 500;
//int winHeight = 500;
//
//char fileName[255];
//ifstream openFile;
//
//vector<vec4> vertex;
//vector<vec4> frag;
//
//vec4* pos;
//vec4* colors;
//vec3* normal;
//
//int index = 0;
//int numVert = 0;
//float gTime = 0.0f;
//
//GLuint vvao;
//GLuint bbuffer;
//
//mat4 myLookAt(vec3 eye, vec3 at, vec3 up)
//{
//	// Implement your own look-at function
//	mat4 V(1.0f);
//	vec3 n = at - eye;
//	n /= length(n);
//
//	float a = dot(up, n);
//	vec3 v = up - a * n;
//	v /= length(v);
//
//	vec3 w = cross(n, v);
//
//	mat4 Rw(1.0f);
//
//	Rw[0][0] = w.x;	Rw[0][1] = v.x; Rw[0][2] = -n.x;
//	Rw[1][0] = w.y;	Rw[1][1] = v.y; Rw[1][2] = -n.y;
//	Rw[2][0] = w.z;	Rw[2][1] = v.z; Rw[2][2] = -n.z;
//
//	mat4 Rc(1.0f);
//	for (int i = 0; i < 4; i++)
//		for (int j = 0; j < 4; j++)
//			Rc[i][j] = Rw[j][i];
//
//	mat4 Tc = Translate(-eye.x, -eye.y, -eye.z);
//
//	V = Rc * Tc;
//
//	return V;
//}
//
//void triple(int a, int b, int c) {
//	pos[index] = vertex[a]; colors[index] = vec4(0.5, 0.5, 0.5, 1); index++;
//	pos[index] = vertex[b]; colors[index] = vec4(0.5, 0.5, 0.5, 1); index++;
//	pos[index] = vertex[c]; colors[index] = vec4(0.5, 0.5, 0.5, 1); index++;
//}
//
//void setPosition() {
//	for (int i = 0; i < frag.size(); i++) {
//		triple(frag[i].x, frag[i].y, frag[i].z);
//	}
//}
//
//mat4 myPerspective(float fovy, float aspectRatio, float zNear, float zFar)
//{
//	mat4 P(1.0f);
//
//	float rad = fovy * 3.141592 / 180.0;
//
//	float sz = 1 / zFar;
//	float h = zFar * tan(rad / 2);
//
//	float sy = 1 / h;
//	float w = h * aspectRatio;
//	float sx = 1 / w;
//
//	mat4 S = Scale(sx, sy, sz);
//	mat4 M(1.0f);
//
//	float c = -zNear / zFar;
//	M[2][2] = 1 / (c + 1);
//	M[2][3] = -c / (c + 1);
//	M[3][2] = -1;
//	M[3][3] = 0;
//
//	P = M * S;
//
//	return P;
//}
//
//void saveArray(string line) {
//	if (!(line[0] == 'f' || line[0] == 'v'))
//		return;
//
//	istringstream ss(line);
//	string buffer;
//	vector<string> bufArr;
//	
//
//	if (line[0] == 'v') {
//		while (getline(ss, buffer, ' ')) {
//			bufArr.push_back(buffer);
//		}
//		
//		vec3 v;
//
//		v.x = stof(bufArr[1]);
//		v.y = stof(bufArr[2]); 
//		v.z = stof(bufArr[3]);
//
//		vertex.push_back(v);
//	}
//
//	if (line[0] == 'f') {
//		while (getline(ss, buffer, ' ')) {
//			bufArr.push_back(buffer);
//		}
//
//		vec3 v;
//		v.x = stoi(bufArr[1]);
//		v.y = stoi(bufArr[2]);
//		v.z = stoi(bufArr[3]);
//
//		frag.push_back(v);
//	}
//}
//
//void readFile() {
//
//	
//	while (1) {
//		cout << "Input FIle Path: "; 
//		cin >> fileName;
//		
//		openFile.open(fileName);
//		
//		if (!openFile) {
//			cout << "File not Found!\n";
//			continue;
//		}
//		break;
//	}
//	
//	string line;
//
//	if (openFile.is_open()) {
//		while (getline(openFile, line)) {
//			saveArray(line);
//		}
//		openFile.close();
//	}
//
//}
//
//void DrawAxis()
//{
//	glUseProgram(program);
//	mat4 x_a = Translate(1., 0, 0) * Scale(2, 0.01, 0.01);
//	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * x_a);
//	glUniform4f(uColor, 1, 0, 0, 1);
//	cube.Draw(program);
//
//	mat4 y_a = Translate(0, 1., 0) * Scale(0.01, 2, 0.01);
//	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * y_a);
//	glUniform4f(uColor, 0, 1, 0, 1);
//	cube.Draw(program);
//
//	mat4 z_a = Translate(0, 0, 1.) * Scale(0.01, 0.01, 2);
//	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat * z_a);
//	glUniform4f(uColor, 0, 0, 1, 1);
//	cube.Draw(program);
//}
//
//void myInit()
//{
//	cube.Init();
//
//	
//
//	program = InitShader("vshader.glsl", "fshader.glsl");
//	prog_phong = InitShader("vPhong.glsl", "fPhong.glsl");
//	glUseProgram(program);
//
//	
//}
//
//void display() {
//	glEnable(GL_DEPTH_TEST);
//	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//
//	mat4 ViewMat = myLookAt(vec3(2, 2, 2), vec3(0, 0, 0), vec3(0, 1, 0));
//
//	float aspect = winWidth / (float)winHeight;
//	float h = 1;
//
//	//mat4 ProjMat = myOrtho(-h*aspect,h*aspect,-h,h,0,100);
//	mat4 ProjMat = myPerspective(100, aspect, 0.1, 100);
//
//	g_Mat = ProjMat * ViewMat;
//
//	glUseProgram(program);
//	uMat = glGetUniformLocation(program, "uMat");
//	uColor = glGetUniformLocation(program, "uColor");
//
//	DrawAxis();
//
//
//	glUseProgram(prog_phong);
//	GLuint vao;
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//	GLuint vbo;
//	glGenBuffers(1, &vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * numVert + sizeof(vec4) * numVert, NULL, GL_STATIC_DRAW);
//
//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * numVert, pos);
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * numVert, sizeof(vec4) * numVert, colors);
//	glUseProgram(prog_phong);
//	GLuint vPosition = glGetAttribLocation(prog_phong, "vPosition");
//	glEnableVertexAttribArray(vPosition);
//	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
//
//	GLuint vColor = glGetAttribLocation(prog_phong, "vColor");
//	glEnableVertexAttribArray(vColor);
//	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4) * numVert));
//
//	glDrawArrays(GL_TRIANGLES, 0, numVert);
//
//	glutSwapBuffers();
//}
//
//void idle() {
//
//}
//
//
//void reshape(int w, int h)
//{
//	winWidth = w;
//	winHeight = h;
//	glViewport(0, 0, w, h);
//	glutPostRedisplay();
//}
//
//void keyboard(unsigned char ch, int x, int y)
//{
//	
//}
//
//int main(int argc, char ** argv){
//	vec3 tmp;
//	vertex.push_back(tmp);
//
//	readFile();
//
//	//for (int i = 0; i < vertex.size(); i++) {
//	//	cout << vertex[i] << endl;
//	//}
//
//	//for (int i = 0; i < frag.size(); i++) {
//	//	cout << frag[i] << endl;
//	//}
//	
//	numVert = frag.size() * 3;
//	pos = (vec4*)malloc(sizeof(vec4) * numVert);
//	colors = (vec4*)malloc(sizeof(vec4) * numVert);
//
//	setPosition();
//
//	//for (int i = 0; i < numVert; i++) {
//	//	if (i % 3 == 0)
//	//		cout << endl;
//	//	cout << pos[i] << endl;
//	//}
//
//	glutInit(&argc, argv);
//
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//	glutInitWindowSize(winWidth, winHeight);
//
//	glutCreateWindow("OpenGL");
//
//	glewExperimental = true;
//	glewInit();
//
//	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
//		glGetString(GL_SHADING_LANGUAGE_VERSION));
//
//
//	myInit();
//	glutDisplayFunc(display);
//	glutIdleFunc(idle);
//	glutReshapeFunc(reshape);
//	glutKeyboardFunc(keyboard);
//	glutMainLoop();
//
//	return 0;
//}