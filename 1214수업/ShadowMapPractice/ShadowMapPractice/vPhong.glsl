#version 430

in  vec4 vPosition;
in  vec4 vColor;
in  vec3 vNormal; 

out vec3 N3; 
out vec3 L3; 
out vec3 V3;  
out vec4 shadowPos;		

uniform mat4 uModelMat; 
uniform mat4 uViewMat; 
uniform mat4 uProjMat; 
uniform mat4 uShadowMat;

uniform vec4 uLPos; 
uniform vec4 uAmb; 
uniform vec4 uDif; 
uniform vec4 uSpc; 
uniform float uShininess; 

void main()
{
	gl_Position  = uProjMat*uViewMat*uModelMat*vPosition;
   
	vec4 N = uViewMat*uModelMat*vec4(vNormal,0); 
	vec4 L = uViewMat*uLPos - uViewMat*uModelMat*vPosition; 
	vec4 V = vec4(0, 0, 0, 1) - uViewMat*uModelMat*vPosition; 

	N3 = normalize(N.xyz); 
	L3 = normalize(L.xyz); 
	V3 = normalize(V.xyz); 

	shadowPos = uShadowMat*uModelMat*vPosition;	
}
