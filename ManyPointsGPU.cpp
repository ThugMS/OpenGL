#include <vgl.h>
#include <stdio.h>

struct vec2 
{
	float x;
	float y;
};

const int NUM_POINTS = 1000;
vec2 points[NUM_POINTS];

void init() {
	for (int i = 0; i < NUM_POINTS; i++)
	{
		float x = (rand() % 200) / 100.0f - 1.0f;
		float y = (rand() % 200) / 100.0f - 1.0f;
		points[i].x = x;
		points[i].y = y;
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	
	for (int i = 0; i < NUM_POINTS; i++) {
		glVertex2f(points[i].x, points[i].y);
	}

	glEnd();
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