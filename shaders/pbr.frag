out vec4 FragColor;
in vec2 Tex;
in vec3 World;
in vec3 Normal;
in vec3 LightPos;
in vec3 ViewPos;
in vec3 FragPos;

#ifdef HAS_DIFFUSE_TEXTURE
uniform sampler2D diffuseMap;
#else
uniform vec4 diffuseColor;
#endif

#ifdef HAS_NORMAL_TEXTURE
uniform sampler2D normalMap;
#endif

#ifdef HAS_METALROUGHNESS_TEXTURE
uniform sampler2D metalroughnessMap;
#else
uniform vec4 mr;
#endif

uniform vec3 camPos;
uniform vec3 lightPosition;
uniform vec3 lightColor;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
#ifdef HAS_METALROUGHNESS_TEXTURE
   vec3 mr = vec3(texture(metalroughnessMap, Tex));
   float metallic = mr.g;
   float roughness = mr.r;
#else
   float metallic = mr.g;
   float roughness = mr.r;
#endif

#ifdef HAS_DIFFUSE_TEXTURE
   vec3 albedo = vec3(texture(diffuseMap, Tex));
#else
   vec3 albedo = vec3(diffuseColor);
#endif

#ifdef HAS_NORMAL_TEXTURE
   vec3 n = vec3(texture(normalMap, Tex));
   n = normalize(n * 2.0 - 1.0);
#else
   vec3 n = Normal;
#endif
   
   vec3 v = normalize(ViewPos - FragPos);
   
   vec3 f0 = vec3(0.04);   
   f0 = mix(f0, albedo, metallic);
   
   vec3 l = normalize(LightPos - FragPos);
   vec3 h = normalize(v + l);
   float distance = length(LightPos - FragPos);
   float attenuation = 1.0;// / (distance * distance);
   vec3 radiance = lightColor * attenuation;
   
   // Cook-Torrance BRDF
   float ndf = DistributionGGX(n, h, roughness);
   float g = GeometrySmith(n, v, l, roughness);
   vec3 f = fresnelSchlick(clamp(dot(h, v), 0.0, 1.0), f0);
   
   vec3 nominator = ndf * g * f;
   float denominator = 4 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0);
   vec3 specular = nominator / max(denominator, 0.001);
   
   vec3 ks = f;
   vec3 kd = vec3(1.0) - ks;
   
   kd *= 1.0 - metallic;

   float NdotL = max(dot(n, l), 0.0);

   vec3 color = (kd * albedo / PI + specular) * radiance * NdotL;
   
   color = color / (color + vec3(1.0));
   color = pow(color, vec3(1.0/2.2));
   
   FragColor = vec4(color, 1.0);
}
