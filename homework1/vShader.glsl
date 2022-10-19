#version 330

in vec4 vPosition;
in vec4 vColor;

uniform float uTime;
uniform float uWave;


out vec4 color;
out float height;


void main()
{

	float angle1 = 47.0f;
	float rad1 = angle1 /180.0f * 3.141592f;

	float angle2 =  uTime * 5.0f;
	float rad2 = angle2 / 180.0f * 3.141592f;

	mat4 rotX = mat4(1.0f);		
	mat4 rotZ = mat4(1.0f);


	rotX[1][1] = cos(rad1);	rotX[2][1] = -sin(rad1);
	rotX[1][2] = sin(rad1);	rotX[2][2] = cos(rad1);

	rotZ[0][0] = cos(rad2);	rotZ[1][0] = -sin(rad2);
	rotZ[0][1] = sin(rad2);	rotZ[1][1] = cos(rad2);

	float distance = (vPosition.x * vPosition.x + vPosition.y * vPosition.y);
	mat4 waveMat = mat4(1.0f);

	if (distance <= 0.5f && uWave >= 1)
	{
		waveMat[3][2] = sin((uTime + distance  * 30)) * 0.5;
		waveMat[3][2] = waveMat[3][2] * (0.75 - distance * 1.5);
	}
	vec4 wPos = rotX*rotZ*waveMat*vPosition;

	wPos.z = wPos.z;
	gl_Position = wPos;
	color = vColor;
	height = waveMat[3][2];
}