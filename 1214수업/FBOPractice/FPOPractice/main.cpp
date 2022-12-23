#define _CRT_SECURE_NO_WARNINGS

#include <vgl.h>
#include <InitShader.h>

#include "MyCube.h"
#include "MyObject.h"

#include <vec.h>
#include <mat.h>

MyCube cube;
MyObject object; 

GLuint phong_prog;
GLuint tex_prog;


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

GLuint fbo;

void myInit()
{
	cube.Init();
	object.Init("bunny.obj"); 

	phong_prog = InitShader("vPhong.glsl", "fPhong.glsl");
	glUseProgram(phong_prog);

	tex_prog = InitShader("vSimpleTex.glsl", "fSimpleTex.glsl");
	glUseProgram(tex_prog);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo); // 0 : 화면
	
	GLuint tex;
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	
	// 빈 종이 하나를 만든다
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				 NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// COLOR ATTACHMENT 에 종이를 설정
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		tex, 0);

	GLuint depthBuf;
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
		depthBuf);

	GLenum target[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, target);
}

void DrawTexture()
{
	glClearColor(0,0,0,1);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(phong_prog);

	vec3 ePos = vec3(4,2,4);
	
	mat4 ModelMat = RotateY(g_Time*30.0f);
	mat4 ViewMat = myLookAt(ePos, vec3(0,0,0), vec3(0,1,0));
	mat4 ProjMat = myPerspective(30, g_aspect, 0.01, 100.0f);

	g_Mat = ProjMat*ViewMat*ModelMat;
	
	// 1. Define Light Properties
	// 
	vec4 lPos = vec4(2, 2, 0, 1);			 
	vec4 lAmb = vec4(0.5, 0.5, 0.5, 1);		
	vec4 lDif = vec4(1, 1, 1, 1);
	vec4 lSpc = lDif; 

	// 2. Define Material Properties
	//
	vec4 mAmb = vec4(0.3, 0.3, 0.4, 1);		
	vec4 mDif = vec4(0.8, 0.5, 0.5, 1);		
	vec4 mSpc = vec4(0.3, 0.3, 0.2, 1); 
	float mShiny = 100;										//1~100;

	// I = lAmb*mAmb + lDif*mDif*(N·L) + lSpc*mSpc*(V·R)^n; 
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

	glUniformMatrix4fv(uModelMat, 1, true, ModelMat); 
	glUniformMatrix4fv(uViewMat, 1, true, ViewMat); 
	glUniformMatrix4fv(uProjMat, 1, true, ProjMat); 
	glUniform4f(uLPos, lPos[0], lPos[1], lPos[2], lPos[3]); 
	glUniform4f(uAmb, amb[0], amb[1], amb[2], amb[3]); 
	glUniform4f(uDif, dif[0], dif[1], dif[2], dif[3]); 
	glUniform4f(uSpc, spc[0], spc[1], spc[2], spc[3]); 
	glUniform1f(uShininess, mShiny); 

	object.Draw(phong_prog); 

}

void display()
{
	// 프레임버퍼 세팅
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// 그림그릴 곳(Texture) 사이즈 세팅
	glViewport(0,0,512,512);
	g_aspect = 1;
	DrawTexture();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0,0,g_width,g_height);
	g_aspect = g_width / (float)g_height;

	glClearColor(1,1,1,1);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(tex_prog);

	vec3 ePos = vec3(2,2,2);
	
	mat4 ModelMat;
	mat4 ViewMat = myLookAt(ePos, vec3(0,0,0), vec3(0,1,0));
	mat4 ProjMat = myPerspective(30, g_aspect, 0.01, 100.0f);

	g_Mat = ProjMat*ViewMat*ModelMat;
	
	// 3. Send Uniform Variables to the shader				
	//
	GLuint uModelMat = glGetUniformLocation(tex_prog, "uModelMat");
	GLuint uViewMat = glGetUniformLocation(tex_prog, "uViewMat");
	GLuint uProjMat = glGetUniformLocation(tex_prog, "uProjMat"); 
	GLuint uTex = glGetUniformLocation(tex_prog, "uTex");

	glUniformMatrix4fv(uModelMat, 1, true, ModelMat); 
	glUniformMatrix4fv(uViewMat, 1, true, ViewMat); 
	glUniformMatrix4fv(uProjMat, 1, true, ProjMat); 
	glUniform1i(uTex, 0);

	cube.Draw(tex_prog); 


	glutSwapBuffers();
}


void idle()
{
	g_Time += 0.016;
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
