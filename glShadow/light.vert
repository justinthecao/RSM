#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;



// Imports the camera matrix from the main function
uniform mat4 camMatrix;
// Imports the model matrix from the main function

uniform mat4 lightTranslation;


void main()
{
	// calculates current position
	vec3 crntPos = vec3(lightTranslation * vec4(aPos, 1.0f));
	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * vec4(crntPos, 1.0);
}