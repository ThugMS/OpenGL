#define _CRT_SECURE_NO_WARNINGS

#include <vgl.h>
#include <InitShader.h>
#include "MyPlane.h"
#include "MyCube.h"
#include "MyObject.h"

#include <vec.h>
#include <mat.h>

MyCube cube;
MyPlane plane;
MyObject object; 

GLuint fbo;		

GLuint phong_prog;

mat4 g_Mat = mat4(1.0f);

float g_Time = 0;

int g_width = 512;
int g_height = 512;
float g_aspect = 1;


mat4 myLookAt(vec3 eye, vec3 at, vec3 up)
{
	mat4 V = mat4(1.0f);
	
	up = normalize(up);
	vec3 n = normalize(at - eye);
	float a = dot(up, n);
	vec3 v = normalize(up-a*n);
	vec3 w = cross(n, v);

	V[0] = vec4(w, dot(-w, eye));
	V[1] = vec4(v, dot(-v, eye));
	V[2] = vec4(-n, dot(n, eye));
	
	return V;
}

mat4 myPerspective(float angle, float aspect, float zNear, float zFar)
{
	float rad = angle*3.141592/180.0f;
	mat4 V(1.0f);
	float h = 2*zFar*tan(rad/2);
	float w = aspect*h;
	mat4 S = Scale(2/w, 2/h, 1/zFar);

	float c = -zNear/zFar;
	
	mat4 Mpt(1.0f);
	Mpt[2] = vec4(0, 0, 1/(c+1), -c/(c+1));
	Mpt[3] = vec4(0, 0, -1, 0);

	V = Mpt*S;
	return V;
}

void myInit()
{
	cube.Init();
	plane.Init(10,10);
	object.Init("bunny.obj"); 
		
	phong_prog = InitShader("vPhong.glsl", "fPhong.glsl");
	glUseProgram(phong_prog);

	glGenFramebuffers(1, &fbo); 
	glBindFramebuffer(GL_FRAMEBUFFER, fbo); 

	GLuint depthBuf; 
	glGenTextures(1, &depthBuf); 
	glActiveTexture(GL_TEXTURE0);				
	glBindTexture(GL_TEXTURE_2D, depthBuf); 

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
		512, 512, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL); 
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); 

	GLfloat border[] = {1, 1, 1, 1};	
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border); 


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS); 
																	

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuf, 0);

	GLenum drawTarget[] = {GL_NONE}; 
	glDrawBuffers(1, drawTarget);	
}

void display()
{
	glViewport(0,0,512,512);
	
	glClearColor(0,0,0,1);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(phong_prog);

	vec3 ePos = vec3(0,4,6);				
	
	mat4 ModelMat = RotateY(g_Time*30.0f);
	mat4 ViewMat = myLookAt(ePos, vec3(0,0,0), vec3(0,1,0));
//	mat4 ProjMat = myPerspective(30, g_aspect, 0.01, 20.0f);
	mat4 ProjMat = Perspective(30, g_aspect, 1, 20.0f);				

	// 1. Define Light Properties
	vec4 lPos = vec4(4, 4, 0, 1);			
	vec4 lAmb = vec4(0.5, 0.5, 0.5, 1);		
	vec4 lDif = vec4(1, 1, 1, 1);
	vec4 lSpc = lDif; 

	mat4 sViewMat = myLookAt(vec3(lPos.x, lPos.y, lPos.z), vec3(0,0,0), vec3(0,1,0));
	mat4 sProjMat = Perspective(30, g_aspect, 1, 20.0f);	

	// 2. Define Material Properties
	vec4 mAmb = vec4(0.3, 0.3, 0.4, 1);		
	vec4 mDif = vec4(0.8, 0.5, 0.5, 1);		
	vec4 mSpc = vec4(0.3, 0.3, 0.2, 1); 
	float mShiny = 100;										
	vec4 amb = lAmb*mAmb;					
	vec4 dif = lDif*mDif;					
	vec4 spc = lSpc*mSpc; 

	// 3. Send Uniform Variables to the shader				
	//
	GLuint uModelMat = glGetUniformLocation(phong_prog, "uModelMat");
	GLuint uViewMat = glGetUniformLocation(phong_prog, "uViewMat");
	GLuint uProjMat = glGetUniformLocation(phong_prog, "uProjMat"); 
	GLuint uLPos = glGetUniformLocation(phong_prog, "uLPos"); 
	GLuint uAmb = glGetUniformLocation(phong_prog, "uAmb"); 
	GLuint uDif = glGetUniformLocation(phong_prog, "uDif"); 
	GLuint uSpc = glGetUniformLocation(phong_prog, "uSpc"); 
	GLuint uShininess = glGetUniformLocation(phong_prog, "uShininess"); 
	GLuint fShadeWithShadow = glGetSubroutineIndex(phong_prog, GL_FRAGMENT_SHADER, "ShadeWithShadow");
	GLuint fRecordDepth = glGetSubroutineIndex(phong_prog, GL_FRAGMENT_SHADER, "fRecordDepth");


	glBindFramebuffer(GL_FRAMEBUFFER, fbo);	
	glViewport(0,0,512,512);
	
	glClearColor(0,0,0,1);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUniformMatrix4fv(uModelMat, 1, true, ModelMat); 
	glUniformMatrix4fv(uViewMat, 1, true, sViewMat);	
	glUniformMatrix4fv(uProjMat, 1, true, sProjMat);	
	glUniform4f(uLPos, lPos[0], lPos[1], lPos[2], lPos[3]); 
	glUniform4f(uAmb, amb[0], amb[1], amb[2], amb[3]); 
	glUniform4f(uDif, dif[0], dif[1], dif[2], dif[3]); 
	glUniform4f(uSpc, spc[0], spc[1], spc[2], spc[3]); 
	glUniform1f(uShininess, mShiny); 
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &fRecordDepth);	
	
	object.Draw(phong_prog); 
	glUniformMatrix4fv(uModelMat, 1, true, ModelMat*Translate(0,-1,0)*RotateX(-90)*Scale(2,2,2)); 
	plane.Draw(phong_prog);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);	
	glViewport(0,0,512,512);
	
	glClearColor(0,0,0,1);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	GLuint uShadowTex = glGetUniformLocation(phong_prog, "uShadowTex"); 
	GLuint uShadowMat = glGetUniformLocation(phong_prog, "uShadowMat"); 

	mat4 ShadowMat = Translate(0.5, 0.5, 0.49)*Scale(0.5, 0.5, 0.5)*sProjMat*sViewMat;	

	glUniformMatrix4fv(uModelMat, 1, true, ModelMat);		
	glUniformMatrix4fv(uViewMat, 1, true, ViewMat);		
	glUniformMatrix4fv(uProjMat, 1, true, ProjMat);	
	glUniformMatrix4fv(uShadowMat, 1, true, ShadowMat); 
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &fShadeWithShadow);	
	glUniform1i(uShadowTex, 0); 

	object.Draw(phong_prog); 
	glUniformMatrix4fv(uModelMat, 1, true, ModelMat*Translate(0,-1,0)*RotateX(-90)*Scale(2,2,2)); 
	plane.Draw(phong_prog);


	glutSwapBuffers();
}


void idle()
{
	g_Time += 0.016f;
	Sleep(16);					// for vSync
	glutPostRedisplay();
}


void reshape(int w, int h)
{
	g_width = w;
	g_height = h;
	glutPostRedisplay();
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(g_width,g_height);

	glutCreateWindow("Shader Test");

	glewExperimental = true;
	glewInit();

	printf("OpenGL %s, GLSL %s\n",	glGetString(GL_VERSION),
									glGetString(GL_SHADING_LANGUAGE_VERSION));

	myInit();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
}
