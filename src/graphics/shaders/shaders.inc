const char atmosphere_frag[] = R"(#version 460 core
#define PI 3.1415926535897932384626433832795028841971
#define TAU 6.283185307179586476925286766559
in V {
	vec3 pos;
} v;
out vec4 fragColor;

void main(){
	fragColor = vec4(0.0, 1.0, 0.0, 0.3);
})";
const char atmosphere_vert[] = R"(#version 460 core
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
})";
const char bake_py[] = R"(import pathlib

directory = pathlib.Path(__file__).parent.absolute()


def processFile(file: pathlib.Path) -> str:
    with open(file, "r") as f:
        return f"const char {file.name.replace('.', '_')}[] = R\"({f.read()})\";\n"


with open(directory / "shaders.inc", "w") as f:
    f.write("".join(processFile(file) for file in directory.iterdir()))
)";
const char cubesphere_comp[] = R"(#version 460 core
#define PI 3.1415926535897932384626433832795028841971
#define TAU 6.283185307179586476925286766559
// https://forum.unity.com/threads/compute-shader-thread-dispatching.953000/#post-6211889
// https://stackoverflow.com/a/62601514
layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

// https://stackoverflow.com/q/52328129
// https://stackoverflow.com/a/38172697 
struct Element {
    float posX;         // 4
    float posY;         // 8
    float posZ;         // 12
    float texCoordU;    // 16
    float texCoordV;    // 20
};
layout(std430, binding = 0) writeonly buffer vertexBlock { Element vertices[]; };
layout(std430, binding = 1) writeonly buffer triangleIndexBlock { uint triangleIndices[]; };
// layout (binding = 2, r32f) uniform readonly image2D topologyMap;
uniform uint resolution;
uniform vec3 normal;
uniform float radius;


// http://mathproofs.blogspot.com/2005/07/mapping-cube-to-sphere.html
vec3 mapCubePointToSphere(vec3 p)
{
    vec3 p2 = p * p;
    return p*sqrt(1.0-(p2.yxx+p2.zzy)/2.0+p2.yxx*p2.zzy/3.0);
}

// https://gamedev.stackexchange.com/a/114416
vec2 mapUnitSpherePointToUV(vec3 p)
{
    return vec2(atan(-p.x,-p.z)/TAU+0.5,-p.y*0.5+0.5);
}


void main() {
    // TODO: make these uniforms for flexibility
    // float datum = 3389.5; // km, Mars 
    // float highestPointAboveDatum = 21.9; // km +ve , Olympus Mons
    // float lowestPointBelowDatum = 7.152; // km -ve , Hellas Planitia
    // float heightExaggerationFactor = 1;

    vec3 tangent = normal.yzx;
    vec3 bitangent = cross(normal, tangent);
    vec2 t = gl_GlobalInvocationID.xy / vec2(resolution - 1);
    vec3 cubePoint = normal + tangent * 2.0 * (t.x - 0.5f) + bitangent * 2.0 * (t.y - 0.5f);
    vec3 spherePoint = mapCubePointToSphere(cubePoint);
    vec2 uv = mapUnitSpherePointToUV(spherePoint);
    uint vertIndex = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * resolution;
    spherePoint *= radius;
    // float relativeHeight = imageLoad(topologyMap, ivec2(uv*imageSize(topologyMap))).r;
    // vec3 planetPoint = spherePoint * (datum + heightExaggerationFactor*mix(lowestPointBelowDatum, highestPointAboveDatum, relativeHeight));
    vertices[vertIndex].posX = spherePoint.x;
    vertices[vertIndex].posY = spherePoint.y;
    vertices[vertIndex].posZ = spherePoint.z;
    vertices[vertIndex].texCoordU = uv.x;
    vertices[vertIndex].texCoordV = uv.y;
    if (gl_GlobalInvocationID.x < resolution - 1 && gl_GlobalInvocationID.y < resolution - 1) {
        uint triIndex = 6 * (gl_GlobalInvocationID.x * (resolution - 1) + gl_GlobalInvocationID.y);
        triangleIndices[triIndex] = vertIndex;
        triangleIndices[triIndex + 1] = vertIndex + resolution + 1;
        triangleIndices[triIndex + 2] = vertIndex + resolution;
        triangleIndices[triIndex + 3] = vertIndex;
        triangleIndices[triIndex + 4] = vertIndex + 1;
        triangleIndices[triIndex + 5] = vertIndex + resolution + 1;
    }
})";
const char lander_frag[] = R"(#version 460 core
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
})";
const char lander_vert[] = R"(#version 460 core
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
})";
const char mars_frag[] = R"(#version 460 core
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
})";
const char mars_vert[] = R"(#version 460 core
#define TAU 6.283185307179586476925286766559
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
uniform mat4 MVP;
uniform mat4 M;
out V {
   vec3 pos;
   vec2 texCoord;
   vec3 tangentLightDir;
} v;

void main()
{
   gl_Position = MVP * vec4(aPos, 1.0);

   vec3 lightDir = vec3(0.0, 0.0, -1.0);
   vec3 N = normalize((M * vec4(aPos, 1.0)).xyz);
   vec3 T = N.yzx;
   vec3 B = cross(N, T);
   mat3 TBN = mat3(T, B, N);

   v.pos = aPos;
   v.texCoord = aTexCoord;
   v.tangentLightDir = TBN * lightDir;
})";
const char marsTerrain_comp[] = R"(#version 460 core
#define PI 3.1415926535897932384626433832795028841971
#define TAU 6.283185307179586476925286766559
// https://forum.unity.com/threads/compute-shader-thread-dispatching.953000/#post-6211889
// https://stackoverflow.com/a/62601514
layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

// https://stackoverflow.com/q/52328129
// https://stackoverflow.com/a/38172697 
struct Element {
    float posX;         // 4
    float posY;         // 8
    float posZ;         // 12
    float texCoordU;    // 16
    float texCoordV;    // 20
};
layout(std430, binding = 0) buffer vertexBlock { Element vertices[]; };
layout (binding = 2, r32f) uniform readonly image2D topologyMap;
uniform uint resolution;

void main() {
    // TODO: make these uniforms for flexibility
    // float datum = 3389.5; // km, Mars 
    float highestPointAboveDatum = 21.9; // km +ve , Olympus Mons
    float lowestPointBelowDatum = 7.152; // km -ve , Hellas Planitia
    // float heightExaggerationFactor = 1;

    vec2 t = gl_GlobalInvocationID.xy / vec2(resolution - 1);
    uint vertIndex = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * resolution;
    float relativeHeight = imageLoad(topologyMap, ivec2(uv*imageSize(topologyMap))).r;
    float heightAboveDatum = mix(lowestPointBelowDatum, highestPointAboveDatum, relativeHeight);
    vertices[vertIndex].posX = spherePoint.x + heightAboveDatum ;
    vertices[vertIndex].posY = spherePoint.y + heightAboveDatum ;
    vertices[vertIndex].posZ = spherePoint.z + heightAboveDatum ;
})";
const char shaders_inc[] = R"()";
const char test_frag[] = R"(#version 460 core

out vec4 fragColor;

void main()
{
   fragColor = vec4(1.0f, 0.4f, 1.0f, 1.0f);
}
)";
const char test_vert[] = R"(#version 460 core
layout (location = 0) in vec3 v;

void main()
{
   gl_Position = vec4(v.xyz, 1.0);
})";
