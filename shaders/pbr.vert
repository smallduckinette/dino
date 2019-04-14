#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

out vec2 Tex;
out vec3 World;
out vec3 Normal;

void main()
{
   Tex = vec2(aTex.x, aTex.y);
   World = vec3(modelMatrix * vec4(aPos, 1.0f));
   Normal = vec3(normalize(normalMatrix * vec4(aNorm, 1.0f)));

   gl_Position = projectionMatrix * viewMatrix * vec4(World, 1.0);
}
