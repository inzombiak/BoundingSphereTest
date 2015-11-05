#include "RenderManager.h"
#define FREEGLUT_STATIC
#define GLEW_STATIC
#include <gl\glew.h>
#include <gl\freeglut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "RenderComponent.h"
#include "HelperFunctions.h"

void RenderManager::Init()
{
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	std::vector<GLuint> shaderList;
	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, ReadFileToString("VertexShader.glsl")));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, ReadFileToString("FragmentShader.glsl")));

	m_program = CreateProgram(shaderList);
	
}
void RenderManager::Clear()
{
	m_renderComponents.clear();
}

void RenderManager::Draw(glm::vec3 position, glm::vec3 direction, glm::vec3 up, bool is3D)
{
	glUseProgram(m_program);

	glm::mat4 mvp(1.0f);

	if (is3D)
	{
		glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(
			position,
			position + direction,
			up
			);
		glm::mat4 model(1.0f);

		mvp = projection*view*model;
	}
	

	glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (unsigned int i = 0; i < m_renderComponents.size(); ++i)
		m_renderComponents[i].Draw(mvp);

	glutSwapBuffers();
}

RenderComponent* RenderManager::CreateRenderComponent()
{
	RenderComponent newComponent;
	newComponent.SetProgram(m_program);
	m_renderComponents.push_back(newComponent);
	return &m_renderComponents[m_renderComponents.size() - 1];
}