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
uniform sampler2D uTexCld;
uniform sampler2D uTexSpec;
uniform sampler2D uTexLight;

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
	float cloud = texture2D(uTexCld, TexCoord - vec2(uTime/20,0)).r;  //구름을 확률로 보고 0~1사이의 숫자를 가져오기 위함
	float spec = texture2D(uTexSpec, TexCoord).r;
	vec4 lightColor = texture2D(uTexLight, TexCoord);


	texColor += (1-cloud)*texColor + cloud*vec4(1,1,1,1); //색의 확률에 따라서 섞어서 사용하는거임
	
	
	float NNL = max(dot(-N,L), 0) * 1.5;
	lightColor = (1-cloud)*lightColor + cloud*vec4(0,0,0,1);



	//fColor = uAmb + uDif*NL + uSpc*VR; 
	//fColor = fColor * texColor;

	fColor = uAmb + texColor*NL + lightColor*NNL + uSpc*VR*spec;
	fColor += pow((1 -max(dot(V,N), 0)), 5) * vec4(0.5,0.5,1,1);			//내가 바라보는 방향과 NORMAL이 0일 수록 경계면인 거임

	fColor.w = 1;
	
	//fColor = vec4(TexCoord.x, 0, TexCoord.y, 1);		//x,y,z 로 접근할 수도 있고 U,V 도 가능함

	//fColor = texture2D(uTex, TexCoord);
}
