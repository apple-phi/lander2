#version 460 core
#define TAU 6.283185307179586476925286766559
layout (location = 0) in vec3 aPos;
uniform mat4 MVP;
out V {
   vec3 pos;
} v;

void main()
{
   gl_Position = MVP * vec4(aPos, 1.0);
   gl_PointSize = 10.0;
   v.pos = aPos;
}