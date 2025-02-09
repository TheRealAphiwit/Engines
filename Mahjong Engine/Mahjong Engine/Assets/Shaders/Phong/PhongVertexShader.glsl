#version 410​
uniform mat4 modelMatrix 	    = mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);​
uniform mat4 viewMatrix 	    = mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);​
uniform mat4 projectionMatrix 	= mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);​
uniform vec4 eyePosition	    = vec4(0.0, 5.0, 0.0, 0.0);

// Mesh data incoming​
in vec3 in_Position;​
in vec2 in_UV;​
in vec3 in_Normal;​

// Output data for the fragment shader​
out vec3 v_normal;	// normals transformed to view space​
out vec2 UV_Coord;	// Just passed on​ (linkely not used here but for frag shader)
out vec3 position;	// World coordinates of the vertex​
out vec3 vecToEye;	// Vector from ​

​void main() 
{​
	// Model-View-Projection matrix
	mat4 mvp = modelMatrix * viewMatrix * projectionMatrix;​ // Projects world coordinates into screen coordinates
	gl_Position = mvp * vec4(in_Position, 1);​ // Transforms the vertex position to clip space (for rendering on screen)
	position = (modelMatrix * vec4(in_Position, 1)).xyz;​ // Transforms the vertex position from local (object) space to world space

	// Continue here <---
	v_normal = in_Normal; // just pass along for now​
	UV_Coord = in_UV;​

	// Calculate vector to eye from vertex.​
	vecToEye = normalize((eyePosition.xyz - position.xyz));​
}