//GLSL VERTEX SHADER
#version 330

layout (location = 0) in vec3 position;
void main()
{
	gl_PointSize = 10.f;
	gl_Position.xyz = position;
	gl_Position.w  =  1.0;
}