layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec4 aTan;
layout (location = 3) in vec2 aTex;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

uniform vec3 lightPosition;
uniform vec3 camPos;

out vec2 Tex;
out vec3 World;
out vec3 TanLightPos;
out vec3 TanViewPos;
out vec3 TanFragPos;

void main()
{
   Tex = vec2(aTex.x, aTex.y);
   World = vec3(modelMatrix * vec4(aPos, 1.0f));
   vec3 Normal = vec3(normalize(normalMatrix * vec4(aNorm, 1.0f)));
   vec3 Tangent = vec3(normalize(normalMatrix * aTan));
   vec3 Bitangent = cross(Normal, Tangent) * aTan.w;

   mat3 TBN = transpose(mat3(Tangent, Bitangent, Normal));
   TanLightPos = TBN * lightPosition;
   TanViewPos = TBN * camPos;
   TanFragPos = TBN * World;

   gl_Position = projectionMatrix * viewMatrix * vec4(World, 1.0);
}
