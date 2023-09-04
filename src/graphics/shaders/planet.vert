#version 460 core
layout (location = 0) in vec3 v;
uniform mat4 MVP;
out vec4 vColor;

void main()
{
   gl_Position = MVP * vec4(v, 1.0);
   vColor = vec4(gl_Position.xyz/2.0+1.0, 1.0);
}