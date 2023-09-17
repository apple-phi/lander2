#version 460 core
#define TAU 6.283185307179586476925286766559
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
uniform mat4 MVP;
out V {
   vec2 texCoord;
   vec3 tangentLightDir;
} v;

void main()
{
   vec3 lightDir = vec3(0.0, 0.0, 1.0);

   vec3 N = normalize(aPos);
   vec3 T = N.yzx;
   vec3 B = cross(N, T);
   mat3 TBN = mat3(T, B, N);

   gl_Position = MVP * vec4(aPos, 1.0);
   v.texCoord = aTexCoord;
   v.tangentLightDir = TBN * lightDir;
}