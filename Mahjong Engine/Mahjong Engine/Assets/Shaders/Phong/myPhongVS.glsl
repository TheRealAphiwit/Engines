#version 410
uniform mat4 modelMatrix = mat4(1.0);
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);
uniform vec3 eyePosition;  // Changed from vec4 to vec3 (since w-component is unnecessary)

// Mesh data incoming
in vec3 in_Position;
in vec2 in_UV;
in vec3 in_Normal;

// Output data for the fragment shader
out vec3 v_normal;  // Transformed normals
out vec2 UV_Coord;  
out vec3 position;  // World-space position of the vertex
out vec3 vecToEye;  // Vector from vertex to the camera

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
    vecToEye = normalize(eyePosition - position);
}