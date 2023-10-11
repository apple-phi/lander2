#version 450 core
#define TAU 6.283185307179586476925286766559
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
uniform mat4 MVP;
out V {
   vec3 pos;
} v;

void main()
{
   gl_Position = MVP * vec4(aPos, 1.0);
   v.pos = aPos;
}