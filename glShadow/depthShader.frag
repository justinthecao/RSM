#version 330 core

in vec3 Normal;
in vec3 crntPos;
in vec3 color;


layout(location = 0) out vec4 normalMap;
layout(location = 1) out float fluxMap;
layout(location = 2) out vec4 coordMap;
uniform vec3 lightPos;



void main()
{
    normalMap = vec4(Normal, 1.0f);
    coordMap = vec4(crntPos, 1.0f);


    vec3 lightVec = lightPos - crntPos;

	float dist = length(lightVec);

	float a = .3;
	float b = 0.3;
	float inten = 2.0f/(a * dist * dist + b * dist + 1.0f);
    fluxMap = inten;
}