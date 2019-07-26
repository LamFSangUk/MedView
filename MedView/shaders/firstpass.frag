#version 330

in vec3 color;
out vec4 outcolor;

void main()
{
	outcolor = vec4(color, 1.0);
}