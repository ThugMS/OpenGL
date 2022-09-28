#version 330

in vec4 vPosition;
in vec4 vColor;

uniform float uTime; //uAngle도 가능

out vec4 color;

void main(){
	
	float angleX = 30.0f;
	float radX = angleX / 180.0f * 3.141592f;

	float angleY = uTime * 90.0f;
	float radY = angleY / 180.0f * 3.141592f;

	mat4 rotX = mat4(1.0f);
	mat4 rotY = mat4(1.0f);

	// X축 회전
	rotX[1][1] = cos(radX); 
	rotX[2][1] = -sin(radX);
	rotX[1][2] = sin(radX);
	rotX[2][2] = cos(radX);

	// Y축 회전
	rotY[0][0] = cos(radY);
	rotY[2][0] = sin(radY);
	rotY[0][2] = -sin(radY);
	rotY[2][2] = cos(radY);

	vec4 wPos = rotX * rotY * vPosition;
	
	gl_Position = wPos;
	color = vColor;
}