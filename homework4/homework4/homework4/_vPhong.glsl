#version 330

in vec4 vPosition;
in vec4 vColor;

out vec4 color;

//in  vec4 vPosition;
//in  vec4 vColor;
//in  vec3 vNormal;
//
////out vec4 color;
//
////phong Shading 을 하기위해서 넘기는것
//out vec3 N3;
//out vec3 L3;
//out vec3 V3;
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
	//float angle1 =45.0f;
	//float rad1 = angle1 /180.0f * 3.141592f;
	//
	//float angle2 =  25f;
	//float rad2 = angle2 / 180.0f * 3.141592f;
	//
	//mat4 rotX = mat4(1.0f);		
	//mat4 rotZ = mat4(1.0f);
	//mat4 rotY = mat4(1.0f);
	//
	//rotX[1][1] = cos(rad1);	rotX[2][1] = -sin(rad1);
	//rotX[1][2] = sin(rad1);	rotX[2][2] = cos(rad1);
	//
	//rotY[0][0] = cos(rad1); rotY[2][0] = sin(rad1);
	//rotY[0][2] = -sin(rad1); rotY[2][2] = cos(rad1);
	//
	//rotZ[0][0] = cos(rad2);	rotZ[1][0] = -sin(rad2);
	//rotZ[0][1] = sin(rad2);	rotZ[1][1] = cos(rad2);
	//
	//vec4 wPos = rotY*rotZ*vPosition;
	//
	//gl_Position = wPos;

	gl_Position = vPosition;
	color = vColor;

	//gl_Position  = uProjMat * (uModelMat * vPosition);
	//gl_Position *= vec4(1,1,-1,1);	// z축 방향이 반대임
	//
	//vec4 lPos = uLPos;
	//vec4 vPos = uModelMat * vPosition;
	//
	//vec4 N = uModelMat*vec4(vNormal,0);
	//vec4 L = lPos - vPos;
	//N3 = normalize(N.xyz);
	//L3 = normalize(L.xyz);
	//V3 = normalize(vec3(0,0,0) - vPos.xyz);
	//
	////vec3 R3 = normalize(2*dot(L3, N3)*N3 - L3);
	//
	//
	//// float sp = pow(max(dot(R3,V3),0), uShiness);
	//// 
	//// vec4 amb = uAmb * uLCol;
	//// vec4 dif = uDif * uLCol * max(dot(N3, L3),0);
	//// vec4 spec = uSpec * uLCol * sp;
	////    	      
	//// color = amb + dif + spec;
	//// //color = vec4(vNormal, 1);
}
