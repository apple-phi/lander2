#version 460 core
#define PI 3.1415926535897932384626433832795028841971
#define TAU 6.283185307179586476925286766559
in V {
	vec3 pos;
} v;
out vec4 fragColor;


void main(){
	// vec3 lightDir = vec3(0.0, 0.0, -1.0);
	// vec3 color =  vec3(1);
	// vec3 normal = normalize(dFdx(v.pos) * dFdy(v.pos) - dFdy(v.pos) * dFdx(v.pos));
	// vec3 ambient = 0.3 * color;
	// vec3 diffuse = color * max(dot(normal, lightDir), 0.0);
	fragColor = vec4(0.0, 1.0, 1.0, 1.0);
}