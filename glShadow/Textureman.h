#ifndef Textureman_CLASS_H
#define Textureman_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>

#include"shaderClass.h"

class Textureman
{
public:
	GLuint ID;
	GLenum type;
	GLuint unit;

	Textureman(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);

	// Assigns a Textureman unit to a Textureman
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a Textureman
	void Bind();
	// Unbinds a Textureman
	void Unbind();
	// Deletes a Textureman
	void Delete();
};
#endif