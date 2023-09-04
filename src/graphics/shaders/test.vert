#version 460 core
layout (location = 0) in vec3 v;

void main()
{
   gl_Position = vec4(v.xyz, 1.0);
}