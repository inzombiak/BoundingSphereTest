#pragma once
#define GLEW_STATIC
#include "glew.h"
#include "GL\freeglut.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <vector>

class RenderComponent
{

public:
	//Draws buffer
	void Draw(glm::mat4 mvp);
	//Update vertex buffer
	void SetVertices(std::vector<glm::vec3> vertices);
	//Update color buffer
	void SetColor(std::vector<glm::vec3> color);
	//Set draw type
	void SetDrawPrimitive(GLuint type);
	//Set OpenGL program
	void SetProgram(GLuint program);

private:
	//Number of vertices to be drawn
	int m_numVertices;
	//Program ID
	GLuint m_program;
	//Buffer IDs
	GLuint m_vertexBufferObject = 0;
	GLuint m_colorBufferObject = 0;
	//Draw type
	GLuint m_drawPrimitive;
};

