//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL

#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Textureman.h"
#include"shaderClass.h"
#include"VAOman.h"
#include"VBOman.h"
#include"EBOman.h"
#include"Camera.h"
#include"Model.h"
#include"Mesh.h"
void processInput(GLFWwindow* window);
void moveModel(GLFWwindow* window);
void moveLightPos(GLFWwindow* window);
void renderFromLight(Model model, Shader shadowShader);

void renderQuad();
const unsigned int width = 800;
const unsigned int height = 800;
const double PI = 3.14159265358979323;
const float lightHeight = 3.0f;
const float baseSize = 1.0f;

const float lightSize = 0.2f;

const std::string modelName = "rsm.gltf";

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES        /        COLORS         /    TexCoord       /       NORMALS      //
	-baseSize, 0.0f,  baseSize,			1.0f,1.0f,1.0f,		0.0f,   0.0f,		    0.0f,1.0f, 0.0f,
	-baseSize, 0.0f, -baseSize,			1.0f,1.0f,1.0f,		0.0f,  baseSize,		0.0f,1.0f, 0.0f,
	 baseSize, 0.0f, -baseSize,			1.0f,1.0f,1.0f,		baseSize, baseSize,		0.0f,1.0f, 0.0f,
	 baseSize, 0.0f,  baseSize,			1.0f,1.0f,1.0f,		baseSize,  0.0f,		0.0f,1.0f, 0.0f,
	 baseSize, 2*baseSize-0.3f, baseSize,	1.0f, 0.0f, 0.0f,		0.0f,   0.0f,			-1.0f, 0.0f, 0.0f,
	 baseSize, 0.0f, -baseSize,			1.0f, 0.0f, 0.0f,       0.0f,  baseSize,		-1.0f, 0.0f, 0.0f,
	 baseSize, 0.0f, baseSize,			1.0f, 0.0f, 0.0f,		baseSize, baseSize,		-1.0f, 0.0f, 0.0f,
	 baseSize, 2*baseSize-0.3f, -baseSize,	1.0f, 0.0f, 0.0f,		baseSize,  0.0f,		-1.0f, 0.0f, 0.0f,
	 -baseSize, 0.0f, -baseSize,		0.0f, 1.0f, 0.0f,		0.0f,   0.0f,			0.0f, 0.0f, 1.0f,
	 -baseSize,2*baseSize-0.3f, -baseSize,	0.0f, 1.0f, 0.0f,       0.0f,  baseSize,		0.0f, 0.0f, 1.0f,
	 baseSize, 0.0f, -baseSize,			0.0f, 1.0f, 0.0f,		baseSize, baseSize,		0.0f, 0.0f, 1.0f,
	 baseSize, 2*baseSize-0.3f, -baseSize, 0.0f, 1.0f, .0f,		baseSize,  0.0f,		0.0f, 0.0f, 1.0f,
	 -baseSize, 0.0f, -baseSize,		0.0f, 0.0f, 1.0f,		0.0f,   0.0f,			1.0f, 0.0f, 0.0f,
	 -baseSize,2 * baseSize -0.3f, -baseSize,	0.0f, 0.0f, 1.0f,       0.0f,  baseSize,		1.0f, 0.0f, 0.0f,
	 -baseSize, 0.0f, baseSize,			0.0f, 0.0f, 1.0f,		baseSize, baseSize,		1.0f, 0.0f, 0.0f,
	 -baseSize, 2 * baseSize -0.3f, baseSize, 0.0f, 0.0f, 1.0f,		baseSize,  0.0f,		1.0f, 0.0f, 0.0f
};



// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	4, 5, 6,
	7, 5, 4,
	8,9,10,
	9,10,11,
	12,13,14,
	13,14,15


};

GLfloat lightVertices[] =
{ //     COORDINATES     //
	-lightSize, - 0.01f,  lightSize,
	-lightSize, - 0.01f, -lightSize,
	 lightSize, - 0.01f, -lightSize,
	 lightSize, - 0.01f,  lightSize,
	-lightSize, 0.01f,  lightSize,
	-lightSize, 0.01f, -lightSize,
	 lightSize, 0.01f, -lightSize,
	 lightSize, 0.01f,  lightSize
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

GLfloat vertices1[] =
{ //     COORDINATES        /        COLORS         /    TexCoord       /       NORMALS      //
		0.0f,  baseSize, -baseSize, 	0.0f, 0.0f, 0.0f,		0.0f,   0.0f,		1.0f, 0.0f, 0.0f,
		0.0f, -baseSize, -baseSize,		0.0f, 0.0f, 0.0f,		0.0f,  baseSize,		1.0f, 0.0f, 0.0f,
		0.0f, -baseSize, baseSize,		0.0f, 0.0f, 0.0f,		baseSize, baseSize,		1.0f, 0.0f, 0.0f,
		0.0f,  baseSize, baseSize,		0.0f, 0.0f, 0.0f,		baseSize,  0.0f,		1.0f, 0.0f, 0.0f
};



// Indices for vertices order


GLfloat vertices2[] =
{ //     COORDINATES        /        COLORS         /    TexCoord       /       NORMALS      //
	-baseSize,  baseSize, 0.0f,		0.0f, 0.0f, 0.0f,		0.0f,   0.0f,		0.0f, 0.0f, 1.0f,
	-baseSize, -baseSize, 0.0f,		0.0f, 0.0f, 0.0f,		0.0f,  baseSize,		0.0f, 0.0f, 1.0f,
	 baseSize, -baseSize, 0.0f,		0.0f, 0.0f, 0.0f,		baseSize, baseSize,		0.0f, 0.0f, 1.0f,
	 baseSize,  baseSize, 0.0f,		0.0f, 0.0f, 0.0f,		baseSize,  0.0f,		0.0f, 0.0f, 1.0f
};





void printVec3(glm::vec3 vector) {
	std::cout << vector.x << " " << vector.y << " " << vector.z << std::endl;
}

void printMat4(const glm::mat4& matrix) {
	const float* ptr = glm::value_ptr(matrix); // Get pointer to matrix data

	std::cout << "Matrix contents:" << std::endl;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << ptr[4 * i + j] << "\t"; // Output matrix element
		}
		std::cout << std::endl;
	}
}
glm::mat4 orientationToRotationMatrix(const glm::vec3& orientationVector) {
	// Ensure the orientation vector is normalized
	glm::vec3 normalizedOrientation = glm::normalize(orientationVector);

	// Determine the rotation axis (cross product of z-axis and orientation vector)
	glm::vec3 rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), normalizedOrientation);

	// Calculate the rotation angle (acos(dot product of z-axis and orientation vector))
	float cosAngle = glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), normalizedOrientation);
	float angle = glm::acos(cosAngle);

	// Create the rotation matrix to align z-axis with the orientation vector
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, rotationAxis);

	return rotationMatrix;
}

bool moveCam = true;
float modelSpeed = 0.015f;
float lightSpeed = 0.015f;
glm::vec3 modelPosition;
glm::mat4 modelTranslation;
glm::mat4 modelRotation;
bool firstClick = true;
float sensitivity = 100.0f;
glm::vec3 modelOrientation(0.0f, 0.0f, 1.0f);
Camera camera(width, height, glm::vec3(0.0f, 1.0f, 3.0f));

bool moveLight = false;
bool toggleReturn = false;
glm::vec3 lightPos = glm::vec3(0.0f, lightHeight, 0.0f);
glm::mat4 lightTranslation = glm::translate(glm::mat4(1.0), lightPos);


int main()
{
	// Initialize GLFW
	glfwInit();
	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "Shadows", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);



	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	// Generates Vertex Array Object and binds it
	VAOman VAO1;
	VAO1.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBOman VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBOman EBO1(indices, sizeof(indices));
	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	Shader shaderProgramModel("default.vert", "default.frag");
	


	// Shader for light cube
	Shader lightShader("light.vert", "light.frag");
	// Generates Vertex Array Object and binds it
	VAOman lightVAO;
	lightVAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBOman lightVBO(lightVertices, sizeof(lightVertices));
	// Generates Element Buffer Object and links it to indices
	EBOman lightEBO(lightIndices, sizeof(lightIndices));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();



	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	std::cout << lightPos.x << " " << lightPos.y << " "  << lightPos.z <<  " " << std::endl;
	//glm::mat4 lightModel = glm::mat4(1.0f);
	//lightModel = glm::translate(lightModel, lightPos);


	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);
	glm::mat4 translationMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);

	modelPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 modelModel = glm::mat4(1.0f);
	modelModel = glm::translate(modelModel, modelPosition);
	modelTranslation = glm::mat4(1.0f);

	modelRotation = glm::mat4(1.0f);
	//modelRotation = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	glm::mat4 modelScale = glm::mat4(1.0f);

	
	lightShader.Activate();
	//glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "lightTranslation"), 1, GL_FALSE, glm::value_ptr(lightTranslation));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "lightSize"), lightSize);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "translationMain"), 1, GL_FALSE, glm::value_ptr(translationMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "rotationMain"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "scaleMain"), 1, GL_FALSE, glm::value_ptr(scaleMatrix));
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "isFloor"), true);
	shaderProgramModel.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramModel.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelModel));
	glUniform4f(glGetUniformLocation(shaderProgramModel.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgramModel.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1f(glGetUniformLocation(shaderProgramModel.ID, "lightSize"), lightSize);
	glUniform1i(glGetUniformLocation(shaderProgramModel.ID, "isFloor"), false);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramModel.ID, "translationMain"), 1, GL_FALSE, glm::value_ptr(modelTranslation));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramModel.ID, "rotationMain"), 1, GL_FALSE, glm::value_ptr(modelRotation));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramModel.ID, "scaleMain"), 1, GL_FALSE, glm::value_ptr(modelScale));
	/*
	* I'm doing this relative path thing in order to centralize all the resources into one folder and not
	* duplicate them between tutorial folders. You can just copy paste the resources from the 'Resources'
	* folder and then give a relative path from this folder to whatever resource you want to get to.
	* Also note that this requires C++17, so go to Project Properties, C/C++, Language, and select C++17
	*/
	//std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	//std::string texPath = "/Resources/YoutubeOpenGL 10 - Specular Maps/";

	//// Textures
	//Texture planksTex((parentDir + texPath + "planks.png").c_str(), GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	//planksTex.texUnit(shaderProgram, "tex0", 0);
	//Texture planksSpec((parentDir + texPath + "planksSpec.png").c_str(), GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
	//planksSpec.texUnit(shaderProgram, "tex1", 1);

	// Original code from the tutorial
	Textureman planksTex("planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	planksTex.texUnit(shaderProgram, "tex0", 0);
	Textureman planksSpec("planksSpec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
	planksSpec.texUnit(shaderProgram, "tex1", 1);


	glUniform1i(glGetUniformLocation(shaderProgram.ID, "tex2"), 2);
	




	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	std::string current = (fs::current_path()).string();
	Model model((current + "/models/balls/" + modelName).c_str());

	Textureman ballTex((current + "/models/balls/baseColor.png").c_str(), GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	ballTex.texUnit(shaderProgramModel, "tex0", 0);
	glUniform1i(glGetUniformLocation(shaderProgramModel.ID, "tex2"), 2);

	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
	
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shader shadowShader("depthShader.vert", "depthShader.frag");
	Shader debugDepthQuad("quad.vs", "quad.fs");
	
	auto t1 = std::chrono::high_resolution_clock::now();
	int count = 0;

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		count += 1;
		auto t2 = std::chrono::high_resolution_clock::now();
		auto t3 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		if (t3 > 1000) {
			std::cout << count << std::endl;
			count = 0;
			t1 = std::chrono::high_resolution_clock::now();
		}
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		if (moveLight) {
			moveLightPos(window);
			lightShader.Activate();
			glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "lightTranslation"), 1, GL_FALSE, glm::value_ptr(lightTranslation));

			shaderProgram.Activate();
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

			shaderProgramModel.Activate();
			glUniform3f(glGetUniformLocation(shaderProgramModel.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

		}
		else {
			if (moveCam){

				camera.Inputs(window);
				
			}

			else {
				moveModel(window);
				shaderProgramModel.Activate();
				glUniformMatrix4fv(glGetUniformLocation(shaderProgramModel.ID, "translationMain"), 1, GL_FALSE, glm::value_ptr(modelTranslation));
				glUniformMatrix4fv(glGetUniformLocation(shaderProgramModel.ID, "rotationMain"), 1, GL_FALSE, glm::value_ptr(modelRotation));

			}
		}

		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		processInput(window);

		//DEPTH MAP
		shadowShader.Activate();

		glm::mat4 projectionMat = glm::perspective(glm::radians(150.0f), 1.0f, glm::length(lightPos - modelPosition) -1.2f, glm::length(lightPos - modelPosition) + 1.0f);
		//printMat4(projectionMat);
		glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(lightPos.x, 0.0f, lightPos.z), glm::vec3(0.0f, 0.0f, -1.0f));
		//printMat4(lightView);
		glm::mat4 lightProjMat = projectionMat * lightView;
	/*	printVec3(lightPos);
		printMat4(lightProjMat);*/

		glUniformMatrix4fv(glGetUniformLocation(shadowShader.ID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightProjMat));
		
		glViewport(0,0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		{
			glUniformMatrix4fv(glGetUniformLocation(shadowShader.ID, "translationMain"), 1, GL_FALSE, glm::value_ptr(modelTranslation));
			model.Draw(shadowShader, camera);
			/*VAO1.Bind();
			glUniformMatrix4fv(glGetUniformLocation(shadowShader.ID, "translationMain"), 1, GL_FALSE, glm::value_ptr(translationMatrix));
			glUniformMatrix4fv(glGetUniformLocation(shadowShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(translationMatrix));
			glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);*/

		}
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

		
		////--------------------------------
		/*glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		debugDepthQuad.Activate();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		renderQuad();*/

		
		//RENDER REALLY NOW ------------------------------------
		// 
		//return back to og
		glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tells OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		// Export the camMatrix to the Vertex Shader of the pyramid
		camera.Matrix(shaderProgram, "camMatrix");
		// Binds textures so that they appear in the rendering
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightProjMat));
		glUniform2f(glGetUniformLocation(shaderProgram.ID, "shadowMapSize"), SHADOW_WIDTH, SHADOW_HEIGHT);
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "shadowFull"), toggleReturn);

		planksTex.Bind();
		planksSpec.Bind();
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		
		shaderProgramModel.Activate();

		ballTex.Bind();
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glUniform3f(glGetUniformLocation(shaderProgramModel.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramModel.ID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightProjMat));
		glUniform2f(glGetUniformLocation(shaderProgramModel.ID, "shadowMapSize"), SHADOW_WIDTH, SHADOW_HEIGHT);
		glUniform1i(glGetUniformLocation(shaderProgramModel.ID, "shadowFull"), toggleReturn);

		camera.Matrix(shaderProgramModel, "camMatrix");
		model.Draw(shaderProgramModel, camera);
		//camera.Matrix(lightShader, "camMatrix");

		// Tells OpenGL which Shader Program we want to use
		lightShader.Activate();
		// Export the camMatrix to the Vertex Shader of the light cube
		camera.Matrix(lightShader, "camMatrix");
		// Bind the VAO so OpenGL knows to use it
		lightVAO.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		//-------------------Finish */
		
		//printMat4(camera.cameraMatrix);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	planksTex.Delete();
	planksSpec.Delete();
	shaderProgram.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}

void renderFromLight(Model model, Shader shadowShader){
	//i need to send a model (translation of model)
	glUniformMatrix4fv(glGetUniformLocation(shadowShader.ID, "translationMain"), 1, GL_FALSE, glm::value_ptr(modelTranslation));
	model.Draw(shadowShader, camera);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_INSERT) == GLFW_PRESS) {
		moveLight = false;
		moveCam = true;
	}
	if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS) {
		moveLight = false;
		moveCam = false;
	}
	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
		moveLight = true;
	}

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		toggleReturn = true;
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		toggleReturn = false;
	}
	//if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		

}

void moveLightPos(GLFWwindow* window)
{
	glm::vec3 orientation = camera.Orientation;
	orientation *= glm::vec3(1.0f, 0.0f, 1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		lightPos += lightSpeed * orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		lightPos += lightSpeed * -glm::normalize(glm::cross(orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		lightPos += lightSpeed * -orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		lightPos += lightSpeed * glm::normalize(glm::cross(orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		lightPos += lightSpeed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		lightPos += lightSpeed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		lightSpeed = 0.03f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		lightSpeed = 0.015f;
	}
	lightPos.y = std::max(2*baseSize-0.2f, lightPos.y);
	lightPos.y = std::min(5.0f, lightPos.y);
	lightPos.x = glm::clamp(lightPos.x, -baseSize+0.1f, baseSize - 0.1f);
	lightPos.z = glm::clamp(lightPos.z, -baseSize+0.1f, baseSize - 0.1f);

	lightTranslation = glm::translate(glm::mat4(1.0), lightPos);

}

void moveModel(GLFWwindow* window)
{
	glm::vec3 orientation = camera.Orientation;
	orientation *= glm::vec3(1.0f, 0.0f, 1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		modelPosition += modelSpeed * orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		modelPosition += modelSpeed * -glm::normalize(glm::cross(orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		modelPosition += modelSpeed * -orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		modelPosition += modelSpeed * glm::normalize(glm::cross(orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		modelPosition += modelSpeed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		modelPosition += modelSpeed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		modelSpeed = 0.03f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		modelSpeed = 0.015f;
	}
	modelPosition.y = std::max(-0.2f, modelPosition.y);
	modelPosition.y = std::min(0.7f, modelPosition.y);
	modelTranslation = glm::translate(glm::mat4(1.0), modelPosition);
	//
	//// Handles mouse inputs
	//if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	//{
	//	// Hides mouse cursor
	//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	//	// Prevents camera from jumping on the first click
	//	if (firstClick)
	//	{
	//		glfwSetCursorPos(window, (width / 2), (height / 2));
	//		firstClick = false;
	//	}

	//	// Stores the coordinates of the cursor
	//	double mouseX;
	//	double mouseY;
	//	// Fetches the coordinates of the cursor
	//	glfwGetCursorPos(window, &mouseX, &mouseY);

	//	// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
	//	// and then "transforms" them into degrees 
	//	float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
	//	float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

	//	// Calculates upcoming vertical change in the Orientation
	//	glm::vec3 newOrientation = glm::rotate(modelOrientation, glm::radians(-rotX), glm::normalize(glm::cross(modelOrientation, Up)));

	//	// Decides whether or not the next vertical Orientation is legal or not
	//	if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
	//	{
	//		modelOrientation = newOrientation;
	//	}

	//	// Rotates the Orientation left and right
	//	modelOrientation = glm::rotate(modelOrientation, glm::radians(-rotY), Up);

	//	// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
	//	glfwSetCursorPos(window, (width / 2), (height / 2));
	//	// Create the rotation matrix to align z-axis with the orientation vector
	//	modelRotation = orientationToRotationMatrix(modelOrientation);
	//}
	//else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	//{
	//	// Unhides cursor since camera is not looking around anymore
	//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	//	// Makes sure the next time the camera looks around it doesn't jump
	//	firstClick = true;
	//}


	//if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)


}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}