#version 410​
uniform vec4 eyePosition = vec4(0.0, 5.0, 0.0, 0.0);​​
uniform vec4 primaryColorVec4 = vec4(1.0, 1.0, 1.0, 1.0);	// Useful for debugging, and can use .w component to fade items alpha​
uniform vec4 light_ambient = vec4(0.2, 0.2, 0.2, 1);		// Base lighting for all objects regardless of position
uniform vec4 light_diffuse = vec4(1, 0.9, 0.8, 1);​		// Color for diffuse light, let’s make it slightly warm, R G B
uniform vec4 light_specular = vec4(0.9, 0.8, 0.7, 1);​		// For highlights​
uniform vec3 light_position = vec4(10, 25, 15);			// Where in the world is the point light?​ - Hard coded atm
uniform vec3 light_attenuation = vec3(1, 0.1, 0.001);		// Constant, linear and quadratic – distance fading​

// Input data from the fragment shader​
in vec3 v_normal;​
in vec2 UV_Coord;​
in vec3 position;​

// Vector from vertex to eye
in vec3 vecToEye;	

out vec4 fragColor;​​


void main()​
{​
	// placeholder color for all textures​
	vec4 texel = vec4(0.5, 0.5, 0.5, 1);​
	fragColor = vec4(0,0,0,texel.w); // start at 0 lighting​
	
	// Ambient first​
	vec4 materialAmbient = vec4(0.5, 0.5, 0.5, 0.1);
	fragColor.xyz += materialAmbient * light_ambient;​

	// Diffuse part start​
	vec3 lightDirection = normalize(position.xyz - light_position.xyz);​

	// Interpolated mesh normal may not be normalized, fix it.​
	vec3 normalizedNormal = normalize(vec3(normal.xyz));

	float difIntensity = max(dot(lightDirection, normalizedNormal), 0.0);

	// do diffuse and specular only if on the right side​
	if (difIntensity > 0) 
	{​
		vec3 diffusePart = difIntensity * light_diffuse.xyz​ * materialDiffuse.xyz;​
		float distance = length(light_position.xyz - position.xyz);​
		float attenuation = 1 / (light_attenuation.x + light_attenuation.y * distance + light_attenuation.z * pow(distance, 2)); 

		// Add the diffuse part​
		fragColor.xyz += diffusePart * attenuation;​

		// Time for specular part..!​
		vec3 vectorToEye = normalize(vecToEye); 

		vec3 halfVector = normalize((-lightDirection + vectorToEye) / 2);​
		float initialBrightness = max(dot(halfVector, normalizedNormal), 0.0);​
		float totalBrightness = initialBrightness;​

		for (int i = 0; i < materialShininess; ++i)
		{​
			totalBrightness *= initialBrightness;​
		}

		// Add the specular part, taking​
		fragColor.xyz += totalBrightness * light_specular.xyz​ * attenuation * materialSpecular.xyz;​

		// If you want for debugging or entity-specific properties​
		//fragColor *= primaryColorVec4;
	}
}