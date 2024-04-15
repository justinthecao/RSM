#include"VAOman.h"

// Constructor that generates a VAOman ID
VAOman::VAOman()
{
	glGenVertexArrays(1, &ID);
}

// Links a VBO Attribute such as a position or color to the VAOman
void VAOman::LinkAttrib(VBOman& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

// Binds the VAOman
void VAOman::Bind()
{
	glBindVertexArray(ID);
}

// Unbinds the VAOman
void VAOman::Unbind()
{
	glBindVertexArray(0);
}

// Deletes the VAOman
void VAOman::Delete()
{
	glDeleteVertexArrays(1, &ID);
}