#version 330 core
// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Texture Coordinates
layout (location = 2) in vec2 aTex;
// Normals (not necessarily normalized)
layout (location = 3) in vec3 aNormal;

// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the current position for the Fragment Shader
out vec3 crntPos;


uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform mat4 translationMain;
uniform mat4 camMatrix; //not used


void main(){
    gl_Position = lightSpaceMatrix * translationMain * model * vec4(aPos, 1.0);
    //gl_Position =  model * vec4(aPos, 1.0);
    Normal = aNormal;
    crntPos = aPos;
}