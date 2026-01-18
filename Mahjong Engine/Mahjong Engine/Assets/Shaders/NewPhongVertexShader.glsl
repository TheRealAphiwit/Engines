#version 410
#define MAX_SHADOWS 8 // needed for handling light space positions

uniform mat4 modelMatrix = mat4(1.0);
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);
uniform vec3 eyePosition;  
uniform int shadowCount;
uniform mat4 lightSpaceMatrices[MAX_SHADOWS];

// Mesh data incoming
layout (location = 0) in vec3 in_Position; // shares same layout locations as ShadowVert - may help?
layout (location = 1) in vec2 in_UV;
layout (location = 2) in vec3 in_Normal;

// Output data for the fragment shader
out vec2 UV_Coord;  
out vec3 v_normal;  // Transformed normals
out vec3 position;  // World-space position of the vertex
out vec3 vecToEye;  // Vector from vertex to the camera
out vec4 fragPosLightSpace[MAX_SHADOWS]; // Light space positions for shadow mapping

void main()
{
    // Corrected MVP order
    mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
    gl_Position = mvp * vec4(in_Position, 1.0);

    // Convert position to world-space
    position = (modelMatrix * vec4(in_Position, 1.0)).xyz;

    // Correct normal transformation
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    v_normal = normalize(normalMatrix * in_Normal);

    // Pass through UV coordinates
    UV_Coord = in_UV;

    // Compute vector to the eye (camera)
    vecToEye = eyePosition - position; // No normalization here now - only in fragment GLSL

    // Compute light space positions for shadow mapping
    for (int i = 0; i < shadowCount && i < MAX_SHADOWS; ++i)
    {
        fragPosLightSpace[i] = lightSpaceMatrices[i] * vec4(position, 1.0); // should be correct since position is already in world space
    }
}