#include <vgl.h>
#include <stdio.h>
#include <InitShader.h>

struct vec2
{
	float x;
	float y;
};

const int NUM_POINTS = 60000;
vec2 points[NUM_POINTS];

void init() {

	// 1. Generate data in CPU
	for (int i = 0; i < NUM_POINTS; i++)
	{
		float x = (rand() % 200) / 100.0f - 1.0f;
		float y = (rand() % 200) / 100.0f - 1.0f;
		points[i].x = x;
		points[i].y = y;
	}

	// 2. Sending the Data to GPU : VAO --> VBO
	// 2-1. Create Vertex Array Object
	GLuint vao;		// OpenGl Unsigned int
	glGenVertexArrays(1, &vao); //Create 1 Vertex and Sending Pointer
	glBindVertexArray(vao);

	// 2-2. Create Vertex Buffer Object in the Vertex Array
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// 2-3. Copy Data in CPU into the Buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * NUM_POINTS, points, GL_STATIC_DRAW);

	// 3. Loading Shaders
	GLuint prog = InitShader("vShader.glsl", "fShader.glsl");
	glUseProgram(prog);

	// 4 . Connect the Vertex Shader with the Data
	int vPosition = 0;
	glDisableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	//Float �������� 2���� �ٰž�, normalize�� false�̰�, 0�� �ǹ̴� ���޾Ƽ� ������ �´ٴ� �ǹ�(ũ�Ⱑ 0), �����Ͱ� �ѹ����̾� ����ִٴ� �ǹ�
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_POINTS, 0, NUM_POINTS); //���´� �� �׷��ֶ�� �޽��
	glFlush();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Many Points");

	glewExperimental = true;
	glewInit();

	printf("OpenGL %s\n", glGetString(GL_VERSION));

	init();
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}