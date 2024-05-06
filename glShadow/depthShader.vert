#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform mat4 translationMain;
uniform mat4 camMatrix; //not used


void main(){
    gl_Position = lightSpaceMatrix * translationMain * model * vec4(aPos, 1.0);
    //gl_Position =  model * vec4(aPos, 1.0);
    
}