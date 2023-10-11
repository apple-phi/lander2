#version 450 core
#define PI 3.1415926535897932384626433832795028841971
#define TAU 6.283185307179586476925286766559
in V {
	vec3 pos;
    vec2 texCoord;
    vec3 tangentLightDir;
} v;
layout(binding = 0) uniform sampler2D colorMap;
layout(binding  = 1) uniform sampler2D normalMap; // TODO: impl
out vec4 fragColor;

// https://gamedev.stackexchange.com/a/114416
vec2 mapUnitSpherePointToUV(vec3 p)
{
    return vec2(atan(-p.x,-p.z)/TAU+0.5,-p.y*0.5+0.5);
}


void main(){
	// TODO: impl normal mapping: tranform lighting by TBN to get it in tangent space then sample normal map
	// https://learnopengl.com/Advanced-Lighting/Normal-Mapping
	vec2 uv = mapUnitSpherePointToUV(normalize(v.pos));
	vec3 color =  texture(colorMap, uv).rgb;
	vec3 normal = normalize(texture(normalMap, uv).rgb * 2.0 - 1.0);
	vec3 ambient = 0.0 * color;
	vec3 diffuse = color * max(dot(normal, v.tangentLightDir), 0.0);
	fragColor = vec4(pow(ambient + diffuse, vec3(1.0/2.2)), 1.0);
}