#version 330

in  vec4 vPosition;
in  vec4 vColor;
in  vec3 vNormal;
out vec4 color;

uniform mat4 uProjMat;
uniform mat4 uModelMat;
uniform vec4 uLPos;
uniform vec4 uLCol;
uniform vec4 uAmb;
uniform vec4 uDif;
uniform vec4 uSpec;
uniform float uShiness;
W
void main()
{
	gl_Position  = uProjMat * (uModelMat * vPosition);
	gl_Position *= vec4(1,1,-1,1);	// z축 방향이 반대임

	vec4 lPos = uLPos;
	vec4 vPos = uModelMat * vPosition;

	vec4 N = uModelMat * vec4(vNormal, 0);
	vec4 L = lPos - vPos;

	vec3 N3 = normalize(N.xyz);
	vec3 L3 = normalize(L.xyz);

	vec3 R3 = normalize(2*dot(L3, N3)*N3 - L3);
	vec3 V3 = normalize(vec3(0,0,0) - vPos.xyz);

	float sp = pow(max(dot(R3,V3),0), uShiness);

	vec4 amb = uAmb * uLCol;
	vec4 dif = uDif * uLCol * max(dot(N3,L3), 0);
	vec4 spec = uSpec * uLCol * sp;
    
	//color = vColor;

	//color = vec4(vNormal, 1);

	color = amb + dif + spec;
}
