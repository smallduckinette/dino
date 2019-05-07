#ifdef HAS_POSITION
layout (location = POSITION) in vec3 aPos;
#endif
#ifdef HAS_NORMAL
layout (location = NORMAL) in vec3 aNorm;
#endif
#ifdef HAS_TEXCOORD_0
layout (location = TEXCOORD_0) in vec2 aTex;
#endif
#ifdef HAS_TANGENT
layout (location = TANGENT) in vec4 aTan;
#endif

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

uniform vec3 lightPosition;
uniform vec3 camPos;

out vec2 Tex;
out vec3 World;
out vec3 LightPos;
out vec3 ViewPos;
out vec3 FragPos;
out vec3 Normal;

void main()
{
#ifdef HAS_TEXCOORD_0
   Tex = vec2(aTex.x, aTex.y);
#endif
   World = vec3(modelMatrix * vec4(aPos, 1.0f));
   Normal = vec3(normalize(normalMatrix * vec4(aNorm, 1.0f)));

#ifdef HAS_NORMAL_TEXTURE
   vec3 Tangent = vec3(normalize(normalMatrix * aTan));
   vec3 Bitangent = cross(Normal, Tangent) * aTan.w;

   mat3 TBN = transpose(mat3(Tangent, Bitangent, Normal));
   LightPos = TBN * lightPosition;
   ViewPos = TBN * camPos;
   FragPos = TBN * World;
#else
   LightPos = lightPosition;
   ViewPos = camPos;
   FragPos = World;
#endif

   gl_Position = projectionMatrix * viewMatrix * vec4(World, 1.0);
}
