#version 410
#define MAX_LIGHTS 16

uniform sampler2D diffuseMap; // New
uniform vec4 eyePosition; // Safe to delete? Since vecToEye is used and VS.glsl uses vec3 instead
uniform vec4 primaryColorVec4; 
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;
uniform vec3 light_position;
uniform vec3 light_attenuation;

// Material properties (added)
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform float materialShininess;

// Input from vertex shader
in vec3 v_normal;
in vec2 UV_Coord;
in vec3 position;
in vec3 vecToEye;

out vec4 fragColor;

struct Light {
    int Type; // 0 = point, 1 = directional, 2 = spot
    vec3 Position;
    vec3 Direction;
    vec3 Color;
    float Intensity;
    float Range;
};

// New multiple light support
uniform int lightCount;
uniform Light lights[MAX_LIGHTS];

const int LIGHT_POINT = 0;
const int LIGHT_DIRECTIONAL = 1;
const int LIGHT_SPOT = 2;

void main() 
{
    // Placeholder color (red)
    // vec4 texel = vec4(0.5, 0.5, 0.5, 1); - Commented out since version was made for testing
    vec4 texel = texture(diffuseMap, UV_Coord);
    
    fragColor = vec4(0.0,0,0,texel.w); // Start at 0 lighting

    // Ambient lighting
    vec4 materialAmbient = vec4(0.5, 0.5, 0.5, 0.1);
    fragColor.xyz += materialAmbient.xyz * light_ambient.xyz;

    // Diffuse lighting
    vec3 lightDirection = normalize(light_position - position);
    vec3 normalizedNormal = normalize(v_normal);
    float difIntensity = max(dot(lightDirection, normalizedNormal), 0.0);

    // Do diffuse and specular only if the surface faces the light
    if (difIntensity > 0) 
    {
        float distance = length(light_position - position);
        
        float attenuation = 1.0 / max(0.001, light_attenuation.x + light_attenuation.y * distance + light_attenuation.z * pow(distance, 2)); 
        vec3 diffusePart = difIntensity * light_diffuse.xyz * materialDiffuse.xyz * texel.rgb;
        
        // Apply diffuse lighting
        fragColor.xyz += diffusePart * attenuation;
	vec4 diffuseTexel = texture(diffuseMap, UV_Coord);

        // Specular lighting
        vec3 vectorToEye = normalize(vecToEye);
        vec3 halfVector = normalize(lightDirection + vectorToEye); // If it doesn't work - change to use negative lightDirection instead
        float initialBrightness = max(dot(halfVector, normalizedNormal), 0.0);
        
        // Use pow() instead of a loop
        float totalBrightness = pow(initialBrightness, materialShininess);

        // Apply specular lighting
	// fragColor = vec4(0,0,0, diffuseTexel.w); - I think I added this for debugging before but commenting it out for now
        fragColor.xyz += totalBrightness * light_specular.xyz * attenuation * materialSpecular.xyz;
    }

    // For debugging purp
    // fragColor *= primaryColorVec4;
    // fragColor = vec4(1);
}