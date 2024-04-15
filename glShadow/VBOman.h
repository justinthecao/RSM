#ifndef VBOman_CLASS_H
#define VBOman_CLASS_H

#include<glad/glad.h>

class VBOman
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBOman(GLfloat* vertices, GLsizeiptr size);

	// Binds the VBOman
	void Bind();
	// Unbinds the VBOman
	void Unbind();
	// Deletes the VBOman
	void Delete();
};

#endif