#version 410
uniform vec4 eyePosition;
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

void main() 
{
    // Placeholder color (red)
    vec4 texel = vec4(0.5, 0.5, 0.5, 1);
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
        vec3 diffusePart = difIntensity * light_diffuse.xyz * materialDiffuse.xyz;
        float distance = length(light_position - position);
        
        // Fix potential division by zero
        float attenuation = 1.0 / max(0.001, light_attenuation.x + light_attenuation.y * distance + light_attenuation.z * pow(distance, 2)); 
        
        // Apply diffuse lighting
        // fragColor.xyz += diffusePart * attenuation;

        // Specular lighting
        vec3 vectorToEye = normalize(vecToEye);
        vec3 halfVector = normalize(-lightDirection + vectorToEye);
        float initialBrightness = max(dot(halfVector, normalizedNormal), 0.0);
        
        // Use pow() instead of a loop
        float totalBrightness = pow(initialBrightness, materialShininess);

        // Apply specular lighting
        // fragColor.xyz += totalBrightness * light_specular.xyz * attenuation * materialSpecular.xyz;
    }

    // For debugging purp
    // fragColor *= primaryColorVec4;
    // fragColor = vec4(1);
}