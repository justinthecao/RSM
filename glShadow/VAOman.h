#ifndef VAOman_CLASS_H
#define VAOman_CLASS_H

#include<glad/glad.h>
#include"VBOman.h"

class VAOman
{
public:
	// ID reference for the Vertex Array Object
	GLuint ID;
	// Constructor that generates a VAOman ID
	VAOman();

	// Links a VBO Attribute such as a position or color to the VAOman
	void LinkAttrib(VBOman& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	// Binds the VAOman
	void Bind();
	// Unbinds the VAOman
	void Unbind();
	// Deletes the VAOman
	void Delete();
};

#endif