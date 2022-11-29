#version 330

//in  vec4 color;

in vec4 color;
out vec4 fColor;


//in vec3 N3;
//in vec3 L3;
//in vec3 V3;
//
//out vec4 fColor;
//
//uniform mat4 uProjMat;
//uniform mat4 uModelMat;
//uniform vec4 uLPos;
//uniform vec4 uLCol;
//uniform vec4 uAmb;
//uniform vec4 uDif;
//uniform vec4 uSpec;
//uniform float uShiness;

void main()
{	
	fColor = color;

	//vec3 N = normalize(N3);
	//vec3 L = normalize(L3);
	//vec3 V = normalize(V3);
	//vec3 R = normalize(2 * dot(L, N) * N - L);
	//
	//float d =  max(0,dot(N, L));
	//float s = pow(max(dot(V,R),0), uShiness);
	//
	////cartoon rendering
	//if(d > 0.8) d= 1;
	//else if(d > 0.5) d = 0.8;
	//else if(d > 0.2) d = 0.5;
	//else d = 0;
	//
	//if(s > 0.8) s= 1;
	//else if(s > 0.5) s = 0.8;
	//else if(s > 0.2) s = 0.5;
	//else s = 0;
	//
	//vec4 amb = uAmb * uLCol;
	//vec4 dif = uDif * uLCol * d;
	//vec4 spec = uSpec * uLCol* s; 
	//
	//fColor = amb + dif + spec;
	//
	////실루엣을 표현하기 위한 식
	//if(abs(dot(N,V)) < 0.1)
	//	fColor = vec4(1,0,0,1);
	//
    //// fColor[0] = color[0];
	//// fColor[1] = color[1];
	//// fColor[2] = color[2];
	//// fColor[3] = color[3];


}
