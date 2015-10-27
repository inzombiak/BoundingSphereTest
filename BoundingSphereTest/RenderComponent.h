#pragma once
#define FREEGLUT_STATIC
#define GLEW_STATIC
#include <gl\glew.h>
#include <gl\freeglut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <vector>

class RenderComponent
{

public:
	
	void Draw(glm::mat4 mvp);

	void SetVertices(std::vector<glm::vec3> vertices);
	void SetColor(std::vector<glm::vec3> color);
	void SetDrawPrimitive(GLuint type);
	void SetProgram(GLuint program);

private:
	GLuint m_program;

	GLuint m_vertexBufferObject = 0;
	GLuint m_colorBufferObject = 0;
	//GLuint m_indexBufferObject;

	GLuint m_drawPrimitive;
};

