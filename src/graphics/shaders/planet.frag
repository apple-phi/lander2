#version 460 core
in vec2 vTexCoord;
layout(binding = 0) uniform sampler2D colorMap;
out vec4 fragColor;

void main(){
	fragColor =  texture(colorMap, vTexCoord);
}