//#include <vgl.h>
//#include <vec.h>
//#include <InitShader.h>
//
//const int NumVertices = 36;
//vec4 positions[NumVertices];
//vec4 colors[NumVertices];
//
//vec4 vertex_pos[8] = {
//	vec4(-0.5,-0.5,0.5,1.0),
//	vec4(-0.5, 0.5,0.5,1.0),
//	vec4(0.5, 0.5,0.5,1.0),
//	vec4(0.5,-0.5,0.5,1.0),
//	vec4(-0.5,-0.5,-0.5,1.0),
//	vec4(-0.5, 0.5,-0.5,1.0),
//	vec4(0.5, 0.5,-0.5,1.0),
//	vec4(0.5,-0.5,-0.5,1.0),
//};
//
//vec4 vertex_color[8] = {
//	vec4(0,0,0,1),
//	vec4(1,0,0,1),
//	vec4(0,1,0,1),
//	vec4(0,0,1,1),
//	vec4(1,1,0,1),
//	vec4(0,1,1,1),
//	vec4(1,0,1,1),
//	vec4(1,1,1,1),
//};
//
//int index = 0;
//void quad(int a, int b, int c, int d)
//{
//	positions[index] = vertex_pos[a]; colors[index] = vertex_color[a]; index++;
//	positions[index] = vertex_pos[b]; colors[index] = vertex_color[b]; index++;
//	positions[index] = vertex_pos[c]; colors[index] = vertex_color[c]; index++;
//
//	positions[index] = vertex_pos[a]; colors[index] = vertex_color[a]; index++;
//	positions[index] = vertex_pos[c]; colors[index] = vertex_color[c]; index++;
//	positions[index] = vertex_pos[d]; colors[index] = vertex_color[d]; index++;
//}
//
//void colorCube()
//{
//	quad(1, 0, 3, 2);
//	quad(2, 3, 7, 6);
//	quad(3, 0, 4, 7);
//	quad(6, 5, 1, 2);
//	quad(4, 5, 6, 7);
//	quad(5, 4, 0, 1);
//}
//
//void myInit()
//{
//	colorCube();
//
//	GLuint vao;
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//	GLuint vbo;
//	glGenBuffers(1, &vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(colors), NULL, GL_STATIC_DRAW);
//
//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(colors), colors);
//
//	GLuint program;
//	program = InitShader("vshader.glsl", "fshader.glsl");
//	glUseProgram(program);
//
//	GLuint vPosition = glGetAttribLocation(program, "vPosition");
//	glEnableVertexAttribArray(vPosition);
//	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
//
//	GLuint vColor = glGetAttribLocation(program, "vColor");
//	glEnableVertexAttribArray(vColor);
//	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(positions)));
//
//}
//
//void myDisplay()
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glEnable(GL_DEPTH_TEST);
//	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
//	glutSwapBuffers();
//}
//
//int main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//	glutInitWindowSize(512, 512);
//	glutCreateWindow("Color Cube");
//
//	glewExperimental = true;
//	glewInit();
//
//	myInit();
//	glutDisplayFunc(myDisplay);
//	glutMainLoop();
//
//
//	return 0;
//}