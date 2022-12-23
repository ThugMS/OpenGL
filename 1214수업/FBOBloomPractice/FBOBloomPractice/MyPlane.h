#pragma once

#include <vgl.h>
#include <vec.h>

struct MyPlaneVertex
{
	vec4 position;
	vec4 color;
	vec3 normal;
	vec3 tangent;
	vec2 texCoord;
};

class MyPlane
{
public:
	MyPlane(void);
	~MyPlane(void);

	int NumVertices;

	MyPlaneVertex * Vertices;
	GLuint Init(int hSlice, int vSlice, int uTexWrap=1, int vTexWrap=1, vec4 color=vec4(0.5,0.5,0.5,1));
	void SetPositionAndOtherAttributes(GLuint program);

	GLuint vao;
	GLuint buffer;
	bool bInitialized;

	void Draw(GLuint program);
};



MyPlane::MyPlane(void)
{
	bInitialized = false;
	NumVertices = 0;
	Vertices = NULL;
}

MyPlane::~MyPlane(void)
{
	if(Vertices != NULL) 
		delete [] Vertices;
}

GLuint MyPlane::Init(int hSlice, int vSlice, int uTexWrap, int vTexWrap, vec4 color)
{
	// The Cube should be initialized only once;
	if(bInitialized == true) return vao;

	NumVertices = hSlice*vSlice*2*3;
	Vertices = new MyPlaneVertex [NumVertices];

	int cur = 0;

	float dx = 2.0/hSlice;
	float dy = 2.0/vSlice;
	float du = uTexWrap/float(hSlice);
	float dv = vTexWrap/float(vSlice);

	for(int i=0; i<vSlice; i++)
	{
		for(int j=0; j<hSlice; j++)
		{
			vec3 a(-1+dx*i,     -1+dy*j,     0);
			vec3 b(-1+dx*(i+1), -1+dy*j,     0);
			vec3 c(-1+dx*(i+1), -1+dy*(j+1), 0);
			vec3 d(-1+dx*i,     -1+dy*(j+1), 0);

			vec3 n(0,0,1);
			vec3 t(1,0,0);
			
			vec2 ta(du*i,	  dv*j);
			vec2 tb(du*(i+1), dv*j);
			vec2 tc(du*(i+1), dv*(j+1));
			vec2 td(du*i,     dv*(j+1));
						
			Vertices[cur].position = a;	Vertices[cur].color = color; Vertices[cur].normal = n; Vertices[cur].texCoord = ta; 
			Vertices[cur].tangent = t;cur ++;
			Vertices[cur].position = b;	Vertices[cur].color = color; Vertices[cur].normal = n; Vertices[cur].texCoord = tb;
			Vertices[cur].tangent = t;cur ++;
			Vertices[cur].position = c;	Vertices[cur].color = color; Vertices[cur].normal = n; Vertices[cur].texCoord = tc;
			Vertices[cur].tangent = t;cur ++;

			Vertices[cur].position = c;	Vertices[cur].color = color; Vertices[cur].normal = n; Vertices[cur].texCoord = tc;
			Vertices[cur].tangent = t;cur ++;
			Vertices[cur].position = d;	Vertices[cur].color = color; Vertices[cur].normal = n; Vertices[cur].texCoord = td;
			Vertices[cur].tangent = t;cur ++;
			Vertices[cur].position = a;	Vertices[cur].color = color; Vertices[cur].normal = n; Vertices[cur].texCoord = ta;
			Vertices[cur].tangent = t;cur ++;
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyPlaneVertex)*NumVertices, Vertices, GL_STATIC_DRAW);
	
	bInitialized = true;
	return vao;
}

void MyPlane::SetPositionAndOtherAttributes(GLuint program)
{
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(MyPlaneVertex), BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, sizeof(MyPlaneVertex), BUFFER_OFFSET(sizeof(vec4)));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(MyPlaneVertex), BUFFER_OFFSET(sizeof(vec4)+sizeof(vec4)));

	GLuint vTangent = glGetAttribLocation(program, "vTangent");
	glEnableVertexAttribArray(vTangent);
	glVertexAttribPointer(vTangent, 3, GL_FLOAT, GL_FALSE, sizeof(MyPlaneVertex), BUFFER_OFFSET(sizeof(vec4)+sizeof(vec4)+sizeof(vec3)));

	GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(MyPlaneVertex), BUFFER_OFFSET(sizeof(vec4)+sizeof(vec4)+sizeof(vec3)+sizeof(vec3)));

}


void MyPlane::Draw(GLuint program)
{
	if(!bInitialized) return;			// check whether it is initiazed or not. 
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	SetPositionAndOtherAttributes(program);
	
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}