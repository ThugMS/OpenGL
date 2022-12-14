#version 330

in  vec2 texCoord;

out vec4 fColor;

uniform sampler2D uTex;

void main()
{
	fColor = texture(uTex, texCoord);
	fColor.w = 1;


}
