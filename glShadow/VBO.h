#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glm/glm.hpp>
#include<glad/glad.h>
#include <iostream>
#include<vector>

// Structure to standardize the vertices used in the meshes
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
	void printVec3(glm::vec3 vec) {
		std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
	}
	void printVertex() {
		std::cout << "Position: ";
		printVec3(position);
		std::cout << std::endl; 
		std::cout << "Normal: ";
		printVec3(normal);
		std::cout << std::endl; 
		std::cout << "Color: ";
		printVec3(color);
		std::cout << std::endl;
	}
};



class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO(std::vector<Vertex>& vertices);

	// Binds the VBO
	void Bind();
	// Unbinds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();
};

#endif