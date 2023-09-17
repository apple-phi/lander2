#version 460 core
in V {
    vec2 texCoord;
    vec3 tangentLightDir;
} v;
layout(binding = 0) uniform sampler2D colorMap;
layout(binding  = 1) uniform sampler2D normalMap; // TODO: impl
out vec4 fragColor;


void main(){
	// TODO: impl normal mapping: tranform lighting by TBN to get it in tangent space then sample normal map
	// https://learnopengl.com/Advanced-Lighting/Normal-Mapping
	vec3 color =  texture(colorMap, v.texCoord).rgb;
	vec3 normal = normalize(texture(normalMap, v.texCoord).rgb * 2.0 - 1.0);
	vec3 ambient = 0.1 * color;
	vec3 diffuse = color * max(dot(normal, v.tangentLightDir), 0.0);
	fragColor = vec4(ambient + diffuse, 1.0);
}