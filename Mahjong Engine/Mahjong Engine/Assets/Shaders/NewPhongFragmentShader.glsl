#version 330 core
#define MAX_LIGHTS 16
#define MAX_SHADOWS 8 // also defined here for consistency

struct Light 
{
    int Type; // 0 = point, 1 = directional, 2 = spot
    vec3 Position;
    vec3 Direction;
    vec3 Color;
    float Intensity;
    float Range;
    float InnerCone;
    float OuterCone;
};

// Lights
uniform int lightCount;
uniform Light lights[MAX_LIGHTS];

const int LIGHT_POINT = 0;
const int LIGHT_DIRECTIONAL = 1;
const int LIGHT_SPOT = 2;

// Material properties
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform float materialShininess;
uniform sampler2D specularMap; // used for lighting and shadow calculations
uniform sampler2D diffuseMap; // visual only - not used for calculations

// Input from vertex shader
in vec3 v_normal;
in vec2 UV_Coord;
in vec3 position;
in vec3 vecToEye;

// Shadows
uniform int shadowCount;
uniform sampler2D shadowMaps[MAX_SHADOWS];

// Shadow inputs from vertex shader
in vec4 fragPosLightSpace[MAX_SHADOWS];

// Output
out vec4 fragColor;

// Shadow calculation function 
float CalculateShadow(int index, vec3 normal, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace[index].xyz / fragPosLightSpace[index].w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0)
        return 0.0;

    float closestDepth = texture(shadowMaps[index], projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    vec4 texel = texture(specularMap, UV_Coord);
    vec3 N = normalize(v_normal);
    vec3 V = normalize(vecToEye);

    vec3 result = vec3(0.0);

    // Ambient
    vec3 ambient = texel.rgb;
    result += ambient;

    for (int i = 0; i < lightCount && i < MAX_LIGHTS; ++i)
    {
        // Shadow
        float shadow = 0.0;

        Light light = lights[i];

        vec3 L;
        float attenuation = 1.0;

        // --- Light direction & attenuation ---
        if (light.Type == LIGHT_POINT || light.Type == LIGHT_SPOT)
        {
            vec3 toLight = light.Position - position;
            float distance = length(toLight);
            L = normalize(toLight);
		
	        // Light gets weaker the further we are
            attenuation = clamp(1.0 - distance / light.Range, 0.0, 1.0);
        }
        else // Directional
        {
            L = normalize(-light.Direction);

            // New - shadow tweaks [2026-01-16]

            // Directional lights have no attenuation
            attenuation = 1.0;
        }

        // --- Diffuse --- (handle diffuse of normals before calc shadow)
        if (dot(L, N) < 0.0) 
        {
            N = -N;
        }

        float NdotL = max(dot(N, L), light.Intensity); // changed from using light.intensity to 0.0 
        vec3 diffuse = NdotL * light.Color * light.Intensity;

        // --- Specular ---
        vec3 H = normalize(L + V);
        float spec = pow(max(dot(N, H), 0.0), 1);
        vec3 specular = spec * materialSpecular.rgb;

        // --- Spotlight cone ---
        if (light.Type == LIGHT_SPOT)
        {
            float theta = dot(L, normalize(-light.Direction));
            float epsilon = max(light.InnerCone - light.OuterCone, 0.001);
            float coneFactor = clamp((theta - light.OuterCone) / epsilon, 0.0, 1.0);
            attenuation *= coneFactor;
        }

        if (light.Type == LIGHT_DIRECTIONAL)
        {
            // Calculate shadows for directional lights
            for (int i = 0; i < shadowCount && i < MAX_SHADOWS; ++i)
            {
                shadow += CalculateShadow(i, N, L);
            }

            result += (1.0 - shadow) * (specular + diffuse * texel.rgb) * attenuation;   
        }

        result += (specular + diffuse * texel.rgb) * attenuation; // Default calc for any light type but directional shall ignore shadows
     }

    fragColor = vec4(result, texel.a);
    // fragColor = vec4(vec3(1.0 - shadow), 1.0);
}