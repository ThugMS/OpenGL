#version 400

in  vec2 texCoord;
out vec4 fColor;

uniform float uTexWidth;
uniform float uTexHeight;

uniform sampler2D uTex;
uniform sampler2D uOriTex;

subroutine vec4 RenderType(); 
subroutine uniform RenderType uRenderPass;	

subroutine(RenderType)
vec4 RenderPass1()				
{
	vec4 c = texture(uTex, texCoord); 
	float bri = (c.r + c.g + c.b)/3; 

	float thresh = 0.4; 

	float ratio = (bri-thresh)/thresh;
	if(ratio < 0) ratio = 0;

	return c*ratio;
}

subroutine(RenderType) 
vec4 RenderPass2()				
{
	float du = 1/uTexWidth;
	float dv = 1/uTexHeight;
	
	int k = 20;					// kernel size (2*k+1)*(2*k+1)
	
	vec4 c = vec4(0,0,0,0);
	for(int v=-k; v<=k; v++)
		c += texture(uTex, texCoord + vec2(0, v*dv));

	c /= float(  (2*k+1) );
	return c;
}

subroutine(RenderType)
vec4 RenderPass3()				
{
	float du = 1/uTexWidth;
	float dv = 1/uTexHeight;
	
	int k = 20;					// kernel size (2*k+1)*(2*k+1)
	
	vec4 c = vec4(0,0,0,0);
	for(int u=-k; u<=k; u++)
		c += texture(uTex, texCoord + vec2(u*du, 0));

	c /= float(  (2*k+1)  );

	vec4 ori = texture(uOriTex, texCoord); 
	return ori + c;
}

void main()
{
	fColor = uRenderPass();		
	fColor.w = 1; 
}
