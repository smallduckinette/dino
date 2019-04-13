#version 330 core

out vec4 FragColor;
in vec2 Tex;
in vec3 Normal;

uniform sampler2D diffuse;

uniform vec3 lightDirection;
uniform vec3 lightColor;

void main()
{
   vec3 n = normalize(Normal);
   vec3 l = normalize(lightDirection);
   float NdotL = clamp(dot(n, l), 0.001, 1.0);

   vec4 texColor = texture(diffuse, Tex);

   FragColor = vec4(NdotL * lightColor * vec3(texColor), 1.0);
}
