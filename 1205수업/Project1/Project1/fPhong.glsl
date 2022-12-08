#version 330

in vec3 N3; 
in vec3 L3; 
in vec3 V3; 
in vec3 wP;
in vec3 wV;
in vec3 wN;

out vec4 fColor;

uniform mat4 uModelMat; 
uniform mat4 uProjMat; 
uniform vec4 uLPos; 
uniform vec4 uAmb; 
uniform vec4 uDif; 
uniform vec4 uSpc; 
uniform float uShininess; 
uniform samplerCube uTexCube;
uniform vec4 uEPos;

void main()
{
	vec3 N = normalize(N3); 
	vec3 L = normalize(L3); 
	vec3 V = normalize(V3); 
	vec3 H = normalize(V+L); 

    float NL = max(dot(N, L), 0); 
	float VR = pow(max(dot(H, N), 0), uShininess); 

	fColor = uAmb + uDif*NL + uSpc*VR; 
	fColor.w = 1; 

	vec3 viewDir = wP - wV;
	vec3 wNormal = normalize(wN);

	//ºûÀÇ ¹Ý»ç
	vec3 dir = reflect(viewDir, wNormal);

	////ºûÀÇ ±¼Àý
	//
	//float rfRatio = 1.3f/1.0f;
	//vec3 dir = refract(viewDir, wNormal, rfRatio);
	//
	////critical angleÀ» ³ÑÀ¸´Ï±ñ Á¤¹Ý»ç·Î Ç¥Çö
	//if(length(dir) < 0.1)
	//	dir = reflect(viewDir, wNormal);

	dir.y = -dir.y;
	vec4 rColor = texture(uTexCube, dir);
	
	//float fr = 1.0f;
	
	float fr = 0 + 0.5* pow(max((1 - dot(V, N)), 0),1);
	fColor = (1-fr)*fColor + fr*rColor;

}
