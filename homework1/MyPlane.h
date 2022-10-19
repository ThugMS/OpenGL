#ifndef _MY_PLAIN_H_
#define _MY_PLAIN_H_

#include <vgl.h>
#include <vec.h>

struct MyPlaneVertex {
	vec4 position;
	vec4 color;
	//vec4 height;
};

class MyColorPlane {
public:
	int NUM_LINE_RECTANGLE = 40;
	int NUM_RECTANGLE = NUM_LINE_RECTANGLE * NUM_LINE_RECTANGLE;
	int NUM_POINTS = 6 * NUM_RECTANGLE;
	int index = 0;
	MyPlaneVertex* vertices;
	GLuint vao;
	GLuint vbo;

	void init();
	void update(int _value);
	void setRectangle(int a, int b);
	void draw(GLuint prog, float gTime, int gWave);
	void setAttributePointers(GLuint prog);
};

void MyColorPlane::init() {
	vertices = new MyPlaneVertex[NUM_POINTS];

	int posX = 0;
	int posY = 0;

	for (int i = 0; i < NUM_LINE_RECTANGLE; i++) {
		for (int j = 0; j < NUM_LINE_RECTANGLE; j++) {
			setRectangle(posX, posY);
			posX++;
		}
		posX = 0;
		posY++;
	}

	glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);
	// 2-2. create vertex buffer object

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyPlaneVertex) * NUM_POINTS,
		vertices, GL_STATIC_DRAW);

	delete[] vertices;
}

void MyColorPlane::update(int _value) {
	if (NUM_LINE_RECTANGLE <= 2 && _value < 0)
		return;

	NUM_LINE_RECTANGLE += _value;
	NUM_RECTANGLE = NUM_LINE_RECTANGLE * NUM_LINE_RECTANGLE;
	NUM_POINTS = 6 * NUM_RECTANGLE;
	index = 0;

	init();
}

void MyColorPlane::setRectangle(int posX, int posY) {

	float interval = 1.5 / (float)NUM_LINE_RECTANGLE;

	vec4 vertex_pos[4] = {
		vec4(-0.75 + ((float)posX * interval), -0.75 + ((float)posY * interval), 0, 1.0),
		vec4(-0.75 + ((float)posX * interval), -0.75 + ((float)(posY + 1) * interval), 0, 1.0),
		vec4(-0.75 + ((float)(posX + 1) * interval), -0.75 + ((float)(posY + 1) * interval), 0, 1.0),
		vec4(-0.75 + ((float)(posX + 1) * interval), -0.75 + ((float)posY * interval), 0, 1.0)
	};

	vec4 vertex_color[4];

	if (posX % 2 == 0 && posY % 2 == 0) {
		for (int i = 0; i < 4; i++) {
			vertex_color[i] = vec4(0.25, 0.25, 0.25, 1);
		}
	}
	else if (posX % 2 == 1 && posY % 2 == 1) {
		for (int i = 0; i < 4; i++) {
			vertex_color[i] = vec4(0.25, 0.25, 0.25, 1);
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			vertex_color[i] = vec4(0.75, 0.75, 0.75, 1);
		}
	}

	vertices[index].position = vertex_pos[0];	vertices[index].color = vertex_color[0]; index++;
	vertices[index].position = vertex_pos[1];	vertices[index].color = vertex_color[1]; index++;
	vertices[index].position = vertex_pos[2];	vertices[index].color = vertex_color[2]; index++;

	vertices[index].position = vertex_pos[2];	vertices[index].color = vertex_color[2]; index++;
	vertices[index].position = vertex_pos[3];	vertices[index].color = vertex_color[3]; index++;
	vertices[index].position = vertex_pos[0];	vertices[index].color = vertex_color[0]; index++;
}

void MyColorPlane::draw(GLuint prog, float gTime, int gWave) {
	glBindVertexArray(vao);

	glUseProgram(prog);
	setAttributePointers(prog);
	GLuint uTime = glGetUniformLocation(prog, "uTime");
	glUniform1f(uTime, gTime);

	GLuint uWave = glGetUniformLocation(prog, "uWave");
	glUniform1f(uWave, (float)gWave);

	glDrawArrays(GL_TRIANGLES, 0, NUM_POINTS);
}

void MyColorPlane::setAttributePointers(GLuint prog) {
	GLuint vPosition = glGetAttribLocation(prog, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, FALSE,
		sizeof(MyPlaneVertex), BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(prog, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, FALSE,
		sizeof(MyPlaneVertex), BUFFER_OFFSET(sizeof(vec4)));

	/*GLuint vHeight = glGetAttribLocation(prog, "vHeight");
	glEnableVertexAttribArray(vHeight);
	glVertexAttribPointer(vHeight, 1, GL_INT, FALSE,
		sizeof(MyPlaneVertex), BUFFER_OFFSET(sizeof(vec4)*2));*/
}

#endif