#include"VBOman.h"

// Constructor that generates a Vertex Buffer Object and links it to vertices
VBOman::VBOman(GLfloat* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

// Binds the VBOman
void VBOman::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbinds the VBOman
void VBOman::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBOman
void VBOman::Delete()
{
	glDeleteBuffers(1, &ID);
}