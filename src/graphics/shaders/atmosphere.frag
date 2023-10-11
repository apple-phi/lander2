#version 450 core
#define PI 3.1415926535897932384626433832795028841971
#define TAU 6.283185307179586476925286766559
in V {
	vec3 pos;
} v;
out vec4 fragColor;

void main(){
	fragColor = vec4(0.0, 1.0, 0.0, 0.3);
}