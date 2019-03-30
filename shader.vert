#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;

void main()
{
   gl_Position = vec4(aPos.x, aPos.y, 0, 1.0) / 5;
}
