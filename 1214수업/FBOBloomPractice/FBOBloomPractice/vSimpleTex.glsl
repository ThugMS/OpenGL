#version 400

in  vec4 vPosition;
in  vec2 vTexCoord;

out vec2 texCoord;

uniform mat4 uModelMat; 
uniform mat4 uViewMat; 
uniform mat4 uProjMat; 

void main()
{
	gl_Position  = uProjMat*uViewMat*uModelMat*vPosition;
	texCoord = vTexCoord;

}
