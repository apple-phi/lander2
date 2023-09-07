#version 460 core
#define TAU 6.283185307179586476925286766559
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
uniform mat4 MVP;
out vec2 vTexCoord;

// // http://mathproofs.blogspot.com/2005/07/mapping-cube-to-sphere.html
// vec3 mapCubePointToSphere(vec3 p)
// {
//     vec3 v = p * p * 0.5f;
//     return p * sqrt(1.0f + vec3(-v.y - v.z + p.y * p.y * p.z * p.z / 3.0f, -v.z - v.x + p.z * p.z * p.x * p.x / 3.0f, -v.x - v.y + p.x * p.x * p.y * p.y / 3.0f));
// }

// vec2 mapUnitSpherePointToUV(vec3 p)
// {
//     vec2 spherePoint;
//     spherePoint.x = atan(p.x, p.z) / TAU + 0.5;
//     spherePoint.y = p.y * 0.5 + 0.5;
//     return spherePoint;
// }

void main()
{
   gl_Position = MVP * vec4(aPos, 1.0);
   vTexCoord = aTexCoord;
}