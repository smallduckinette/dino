#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;

void main()
{
   gl_Position = vec4(aPos.x / 5, aPos.y / 5, aPos.z / 5, 1.0);
}
