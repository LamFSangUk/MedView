#version 330
layout(location = 0) in vec3 position;

uniform mat4 mvp;

out vec3 ray_entry;

void main()
{
  gl_Position = mvp * vec4(position, 1.0);
  ray_entry = position;
}