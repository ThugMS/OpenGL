//#include <vgl.h>
//
//void display()
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//	glBegin(GL_TRIANGLES);
//
//	glColor3f(1, 0, 0);
//	glVertex2f(0.5, -0.5);
//
//	glColor3f(0, 0, 1);
//	glVertex2f(-0.5, 0.5);
//
//	glVertex2f(-0.5, -0.5);
//	
//	glVertex2f(0.5, -0.5);
//	glVertex2f(0.5, 0.5);
//	glVertex2f(-0.5, 0.5);
//	glEnd();
//	glFlush();
//}
//
//int main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
//	glutInitWindowSize(512, 512);
//	glutCreateWindow("Hello GL");
//
//	glutDisplayFunc(display);
//	glutMainLoop();
//
//	return 0;
//}