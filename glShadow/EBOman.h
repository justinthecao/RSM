#ifndef EBOman_CLASS_H
#define EBOman_CLASS_H

#include<glad/glad.h>

class EBOman
{
public:
	// ID reference of Elements Buffer Object
	GLuint ID;
	// Constructor that generates a Elements Buffer Object and links it to indices
	EBOman(GLuint* indices, GLsizeiptr size);

	// Binds the EBOman
	void Bind();
	// Unbinds the EBOman
	void Unbind();
	// Deletes the EBOman
	void Delete();
};

#endif