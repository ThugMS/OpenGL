#version 330
in vec3 N3; 
in vec3 L3; 
in vec3 V3; 
in vec2 TexCoord;

out vec4 fColor;

uniform mat4 uModelMat; 
uniform mat4 uProjMat; 
uniform vec4 uLPos; 
uniform vec4 uAmb; 
uniform vec4 uDif; 
uniform vec4 uSpc; 
uniform float uShininess; 
uniform sampler2D uTex;

uniform float uTime;

void main()
{
	vec3 N = normalize(N3); 
	vec3 L = normalize(L3); 
	vec3 V = normalize(V3); 
	vec3 H = normalize(V+L); 

    float NL = max(dot(N, L), 0); 
	float VR = pow(max(dot(H, N), 0), uShininess); 
	vec4 texColor = texture2D(uTex, TexCoord);

	//fColor = uAmb + uDif*NL + uSpc*VR; 
	//fColor = fColor * texColor;

	fColor = uAmb + texColor*NL + uSpc*VR;

	fColor.w = 1;
	
	//fColor = vec4(TexCoord.x, 0, TexCoord.y, 1);		//x,y,z �� ������ ���� �ְ� U,V �� ������

	//fColor = texture2D(uTex, TexCoord);
}
