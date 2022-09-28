#include <vgl.h>
#include <InitShader.h>
#include <vec.h>

const int NUM_POINTS = 36;

vec4 points[NUM_POINTS];
vec4 colors[NUM_POINTS];

GLuint prog;
float gTime = 0.0f;

vec4 vertex_pos[8] = {
	vec4(-0.5,-0.5,0.5,1.0),
	vec4(-0.5, 0.5,0.5,1.0),
	vec4(0.5, 0.5,0.5,1.0),
	vec4(0.5,-0.5,0.5,1.0),
	vec4(-0.5,-0.5,-0.5,1.0),
	vec4(-0.5, 0.5,-0.5,1.0),
	vec4(0.5, 0.5,-0.5,1.0),
	vec4(0.5,-0.5,-0.5,1.0)
};

vec4 vertex_color[8] = {
	vec4(1,1,1,1),
	vec4(1,0,0,1),
	vec4(0,1,0,1),
	vec4(0,0,1,1),
	vec4(1,1,0,1),
	vec4(1,0,1,1),
	vec4(0,1,1,1),
	vec4(0,0,0,1)
};


void setRectangle(int a, int b, int c, int d)
{
	static int index = 0;
	points[index] = vertex_pos[a];		colors[index] = vertex_color[a]; index++;
	points[index] = vertex_pos[b];		colors[index] = vertex_color[b]; index++;
	points[index] = vertex_pos[c];		colors[index] = vertex_color[c]; index++;

	points[index] = vertex_pos[c];		colors[index] = vertex_color[c]; index++;
	points[index] = vertex_pos[d];		colors[index] = vertex_color[d]; index++;
	points[index] = vertex_pos[a];		colors[index] = vertex_color[a]; index++;
}

void init()
{
	// 1. create data in cpu
	setRectangle(0, 3, 2, 1);
	setRectangle(4, 5, 6, 7);
	setRectangle(0, 4, 7, 3);
	setRectangle(3, 7, 6, 2);
	setRectangle(2, 6, 5, 1);
	setRectangle(0, 1, 5, 4);

	// 2. seding the data to gpu
	// 2-1. create vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// 2-2. create vertex buffer object
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// 2-3. sending the data to the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points),
		sizeof(colors), colors);

	// 3. loading the shaders
	prog =
		InitShader("vShader_CubeRotation.glsl", "fShader_Cube.glsl");
	glUseProgram(prog);
	// 4. connect the data with the shaders
	GLuint vPosition = glGetAttribLocation(prog, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(prog, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));
}

void display()
{
	GLuint uTime = glGetUniformLocation(prog, "uTime");
	glUniform1f(uTime, 0.1f * gTime);

	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, NUM_POINTS);
	glFlush();
}

void myIdle() {
	gTime += 0.001f;
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
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

//È¸Àü 
//
//cos  -sin
//
//sin  cos