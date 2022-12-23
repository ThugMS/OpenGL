#pragma once 

#include <vgl.h>

class MyObject 
{
public:
	MyObject(){}; 
	~MyObject(){};

	int m_NumVertices;		

	GLuint vao; 
	GLuint buffer; 

	void Init(const char * filename); 
	void Draw(GLuint program); 
};