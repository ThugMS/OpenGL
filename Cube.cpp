#include <vgl.h>
#include <InitShader.h>
#include <vec.h>

const int NUM_POINTS = 36;

vec4 points[NUM_POINTS];
vec4 colors[NUM_POINTS];

vec4 vertex_pos[8] = {
	vec4(-0.5, -0.5, 0.5, 1.0),
	vec4(-0.5, 0.5, 0.5, 1.0),
	vec4(0.5, 0.5, 0.5, 1.0),
	vec4(0.5, -0.5, 0.5, 1.0),

	vec4(-0.5, -0.5, -0.5, 1.0),
	vec4(-0.5, -0.5, -0.5, 1.0),
	vec4(0.5, 0.5, -0.5, 1.0),
	vec4(0.5, -0.5, -0.5, 1.0)
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

void setRectangle(int a, int b, int c, int d) {

	static int index = 0;

	points[index] = vertex_pos[a];	colors[index] = vertex_color[a];  index++;
	points[index] = vertex_pos[b];	colors[index] = vertex_color[b];  index++;
	points[index] = vertex_pos[c];	colors[index] = vertex_color[c];  index++;
						  
	points[index] = vertex_pos[c];	colors[index] = vertex_color[c];  index++;
	points[index] = vertex_pos[d];	colors[index] = vertex_color[d];  index++;
	points[index] = vertex_pos[a];	colors[index] = vertex_color[a];  index++;
}

void init() {
	// 1. Create Data in CPU

	//반시계 방향으로 하는게 국룰이다.
	setRectangle(0, 3, 2, 1);
	setRectangle(4, 5, 6, 7);
	setRectangle(0, 4, 7, 3);
	setRectangle(3, 7, 6, 2);
	setRectangle(2, 6, 5, 1);
	setRectangle(0, 1, 5, 4);

	// 2. Sending the Data to GPU
	// 2-1. Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 2-2. Create Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// 2-3. Sending the Data to the Buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW); //NULL 인 이유는 아직 데이터의 공간만큼만 확보하고 데이터를 넘기지는 않음
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points); // 이제부터 확보해놓은 공간에 데이터를 넘기는 것
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

	// 3. Loading the Shaders
	GLuint prog = InitShader("vShader_Cube.glsl", "fShader_Cube.glsl");
	glUseProgram(prog);

	// 4. Connect the Data with the Shaders
	GLuint vPosition = glGetAttribLocation(prog, "vPosition");
	glDisableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(prog, "vColor");
	glDisableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, FALSE, 0, BUFFER_OFFSET(sizeof(points)));
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, NUM_POINTS);
	glFlush();
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
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}