#version 410
uniform Material material;

uniform sampler2D diffuseMap; // New
uniform sampler2D albedoMap; // New

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

struct Material {
    sampler2D albedo;
    sampler2D specular;
    sampler2D normal;
    float shininess;
};

void main() 
{
    // Sampling early in main
    vec3 albedoTexel = texture(albedoMap, UV_Coord);
    vec4 specularTexel = texture(diffuseMap, UV_Coord);

    // Placeholder color (red) - used for early testing or debugging
    // fragColor = vec4(0.0,0,0,diffuseTexel.w); // Start at 0 lighting

    // New color logic
    vec3 color = vec3(0.0);

    // Ambient lighting
    vec4 ambientColor = vec4(1, 0.04, 0.04, 0.1);
    vec3 ambient = albedoTexel * ambientColor.rgb * light_ambient.rgb;
    color += ambient;

    // Diffuse lighting
    vec3 lightDir = normalize(light_position - position);
    vec3 normal = normalize(v_normal);
    float diffIntensity = max(dot(lightDir, normal), 0.0);

    // Do diffuse and specular only if the surface faces the light
    if (diffIntensity > 0) 
    {
        float dist = length(light_position - position);
        float attenuation = 1.0 / max(0.001, light_attenuation.x + light_attenuation.y * distance + light_attenuation.z * pow(distance, 2)); 
        
        vec3 diffuse = difIntensity * light_diffuse.rgb * specularTexel.rgb * materialDiffuse.rgb;
        
        // Apply diffuse lighting
        color += diffuse * attenuation;
	    
        // Specular lighting
        vec3 viewDir = normalize(vecToEye);
        vec3 halfVec = normalize(-lightDir + viewDir);
        float specAngle = max(dot(halfVec, normal), 0.0);
        float specIntensity = pow(specAngle, materialShininess);

        vec3 specular = specIntensity * light_specular.rgb * materialSpecular.rgb * albedoTexel;
        color += specular * attenuation;
    }

    // For debugging purp
    // fragColor *= primaryColorVec4;
    // fragColor = vec4(1);

    fragColor = vec4(color, specularTexel.a);
}