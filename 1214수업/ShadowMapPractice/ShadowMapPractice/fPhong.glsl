#version 430 core

in vec3 N3; 
in vec3 L3; 
in vec3 V3; 
in vec4 shadowPos; 
out vec4 fColor;

uniform mat4 uModelMat; 
uniform mat4 uProjMat; 
uniform vec4 uLPos; 
uniform vec4 uAmb; 
uniform vec4 uDif; 
uniform vec4 uSpc; 
uniform float uShininess; 
uniform sampler2DShadow uShadowTex;		

vec4 phongModel()
{
	vec3 N = normalize(N3); 
	vec3 L = normalize(L3); 
	vec3 V = normalize(V3); 
	vec3 H = normalize(V+L); 

    float NL = max(dot(N, L), 0); 
	float VR = pow(max(dot(H, N), 0), uShininess); 

	return  uDif*NL + uSpc*VR; 
}

subroutine void RenderPassType();
subroutine uniform RenderPassType uRenderPass;

subroutine (RenderPassType)
void ShadeWithShadow()		
{
	vec4 diffAndSpec = phongModel();
	
	float factor = textureProj(uShadowTex, shadowPos);
	
	fColor = diffAndSpec*factor + uAmb;

	// Implement the shadow map
}

subroutine (RenderPassType)
void RecordDepth()				
{								

}


void main()
{
	ShadeWithShadow();
}
