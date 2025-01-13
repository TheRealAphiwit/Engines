#version 410
uniform vec4 eyePosition;
uniform vec4 primaryColorVec4; 
uniform vec4 light_ambient; 
uniform vec4 light_diffuse;
uniform vec4 light_specular;
uniform vec3 light_position; 
uniform vec3 light_attenuation;

uniform vec4 materialAmbient = vec4(0.5, 0.5, 0.5, 1);
uniform vec4 materialDiffuse = vec4(0.7, 0.7, 0.7, 1);
uniform vec4 materialSpecular = vec4(1.0, 1.0, 1.0, 1);
uniform float materialShininess = 32.0;

in vec3 v_normal;
in vec3 position;
in vec3 vecToEye;

out vec4 fragColor;

void main() 
{
    vec3 normalizedNormal = normalize(v_normal);
    vec3 lightDirection = normalize(light_position.xyz - position);
    vec3 vectorToEye = normalize(vecToEye);

    // Ambient
    fragColor = materialAmbient * light_ambient;

    // Diffuse
    float difIntensity = max(dot(lightDirection, normalizedNormal), 0.0);
    if (difIntensity > 0.0) 
    {
        vec3 diffusePart = difIntensity * light_diffuse.xyz * materialDiffuse.xyz;
        float distance = length(light_position.xyz - position);
        float attenuation = 1.0 / (light_attenuation.x + light_attenuation.y * distance + light_attenuation.z * distance * distance);
        fragColor.xyz += diffusePart * attenuation;

        // Specular
        vec3 halfVector = normalize(lightDirection + vectorToEye);
        float specIntensity = pow(max(dot(halfVector, normalizedNormal), 0.0), materialShininess);
        vec3 specularPart = specIntensity * light_specular.xyz * materialSpecular.xyz * attenuation;
        fragColor.xyz += specularPart;
    }

    // fragColor.w = primaryColorVec4.w; // Alpha
    fragColor.w = 0.5;
}