#version 330

in vec4 color;
in float height;

out vec4 fColor;

uniform float uTime;

void main()
{
	fColor = color;

	if(height >= 0.0)
		fColor = color + vec4(51/255, 1.0, 1.0, 1.0) * height * 5;
		
	else
		fColor = color + vec4(-0.5, -0.5, -51/255, 1.0) * height * 5;

}