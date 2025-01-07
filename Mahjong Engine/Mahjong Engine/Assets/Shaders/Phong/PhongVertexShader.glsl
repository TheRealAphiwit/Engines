#version 410​
uniform mat4 modelMatrix 	= mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);​
uniform vec4 eyePosition	= vec4(0.0, 5.0, 0.0, 0.0);​
// The following 2 things can be put into one
uniform mat4 viewMatrix 	= mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);​
uniform mat4 projectionMatrix 	= mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);​


// Mesh data incoming​
in vec3 in_Position;​
in vec2 in_UV;​
in vec3 in_Normal;​


// Output data for the fragment shader​
out vec3 v_normal;	// normals transformed to view space​
out vec2 UV_Coord;	// Just passed on​
out vec3 position;	// World coordinates of the vertex​
out vec3 vecToEye;	// Vector from ​

​void main() 
{​
	mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;​
	gl_Position = mvp * vec4(in_Position, 1);​
	position = (modelMatrix * vec4(in_Position, 1)).xyz;​
	v_normal = in_Normal; // just pass along for now​
	UV_Coord = in_UV;​

	// Calculate vector to eye from vertex.​
	vecToEye = normalize((eyePosition.xyz - position.xyz));​
}