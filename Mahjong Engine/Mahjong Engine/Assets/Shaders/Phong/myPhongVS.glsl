#version 410
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec4 eyePosition;

in vec3 in_Position;
in vec2 in_UV;
in vec3 in_Normal;

out vec3 v_normal;
out vec2 UV_Coord;
out vec3 position;
out vec3 vecToEye;

void main() 
{
    mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
    gl_Position = mvp * vec4(in_Position, 1.0);

    position = (modelMatrix * vec4(in_Position, 1.0)).xyz;
    v_normal = mat3(modelMatrix) * in_Normal; // Transform normals correctly
    UV_Coord = in_UV;

    vecToEye = normalize(eyePosition.xyz - position.xyz);
}