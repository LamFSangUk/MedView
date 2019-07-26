#version 330

layout (location = 0) in vec3 position;

uniform mat4 mvp;

out vec3 color;

void main()
{
	gl_Position = mvp * vec4(position, 1.0);
	
	/* pass postion as color */
	color = position;
}