//#include <vgl.h>
//#include <InitShader.h>
//#include <vec.h>
//#include "1010_MyColorCube.h"
//#include "1010_MyCylinder.h"
//
//float gTime = 0.0f;
//GLuint prog;
//MyColorCube cube;
//MyCylinder cylinder;
//int gDiv = 10;
//
//void init()
//{
//	cube.init();
//	cylinder.init(gDiv);
//
//	prog = InitShader("1010_vShader.glsl", "1010_fShader.glsl");
//	glUseProgram(prog);
//}
//void display()
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glEnable(GL_DEPTH_TEST);
//
//	glUseProgram(prog);
//	GLuint uTime = glGetUniformLocation(prog, "uTime");
//	glUniform1f(uTime, gTime);
//
//	//cube.draw(prog);
//	cylinder.draw(prog);
//	glFlush();
//}
//
//void myIdle()
//{
//	//gTime += 0.001f;
//	//printf("time = %f\n", gTime);
//	gTime += 0.016f;
//	Sleep(16);				// 16 msec = 1/60 sec = 60 Hz
//	glutPostRedisplay();
//}
//
//void myKeyboard(unsigned char c, int x, int y)
//{
//	printf("key = %c, x = %d, y=%d \n", c, x, y);
//	switch (c)
//	{
//	case '1':	gDiv = (gDiv > 3) ? (--gDiv) : 3;		cylinder.init(gDiv);	break;
//	case '2':	gDiv++;								cylinder.init(gDiv);	break;
//	default:				break;
//	}
//}
//
//int main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
//	glutInitWindowSize(512, 512);
//	glutCreateWindow("Hello GL");
//
//	glewExperimental = true;
//	glewInit();
//
//	init();
//	glutIdleFunc(myIdle);
//	glutKeyboardFunc(myKeyboard);
//	glutDisplayFunc(display);
//	glutMainLoop();
//
//	return 0;
//}