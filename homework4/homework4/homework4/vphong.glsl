#version 330

in  vec4 vPosition;
in  vec4 vColor;
in  vec3 vNormal;
in	vec3 vPhNormal;

//out vec4 color;

//phong Shading 을 하기위해서 넘기는것
out vec3 N3;
out vec3 L3;
out vec3 V3;

uniform mat4 uProjMat;
uniform mat4 uModelMat;
uniform vec4 uLPos;
uniform vec4 uLCol;
uniform vec4 uAmb;
uniform vec4 uDif;
uniform vec4 uSpec;
uniform float uShiness;
uniform float uIsFlat;

void main()
{	
	

	gl_Position  = uProjMat * (uModelMat * vPosition);
	gl_Position *= vec4(1,1,-1,1);	// z축 방향이 반대임

	vec4 lPos = uLPos;
	vec4 vPos = uModelMat * vPosition;
	vec4 N;

	if(uIsFlat == 0){
		N = uModelMat*vec4(vPhNormal,0);
	}
	else if(uIsFlat == 1){
		N = uModelMat*vec4(vNormal,0);
	}
	
	//N = uModelMat*vec4(vNormal,0);

	vec4 L = lPos - vPos;
	N3 = normalize(N.xyz);
	L3 = normalize(L.xyz);
	V3 = normalize(vec3(0,0,0) - vPos.xyz);

	//vec3 R3 = normalize(2*dot(L3, N3)*N3 - L3);
	
	
	// float sp = pow(max(dot(R3,V3),0), uShiness);
	// 
	// vec4 amb = uAmb * uLCol;
	// vec4 dif = uDif * uLCol * max(dot(N3, L3),0);
	// vec4 spec = uSpec * uLCol * sp;
	//    	      
	// color = amb + dif + spec;
	// //color = vec4(vNormal, 1);
}
