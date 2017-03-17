#include "RenderManager.h"
#define GLEW_STATIC
#include "glew.h"
#include "gl\freeglut.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "RenderComponent.h"
#include "HelperFunctions.h"

void RenderManager::Init()
{
	//GL flags
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Create shaders and add to vector
	std::vector<GLuint> shaderList;
	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, ReadFileToString("VertexShader.glsl")));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, ReadFileToString("FragmentShader.glsl")));

	//Create program
	m_program = CreateProgram(shaderList);
	
}
void RenderManager::Clear()
{
	//Clear render components
	m_renderComponents.clear();
}

void RenderManager::Draw(glm::vec3 position, glm::vec3 direction, glm::vec3 up, bool is3D)
{
	//Set drawing program
	glUseProgram(m_program);

	//MVP matrix, for 2D drawing this is all we need
	glm::mat4 mvp(1.0f);

	//If were in 3D then camera is "active" and we need to calculate the actual MVP
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
	
	//Clear screen
	glClearColor(213.f / 255.f, 220.f / 255.f, 222.f / 255.f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw all the components
	for (unsigned int i = 0; i < m_renderComponents.size(); ++i)
		m_renderComponents[i].Draw(mvp);

	glutSwapBuffers();
}

RenderComponent* RenderManager::CreateRenderComponent()
{
	//Create a new render component and return it
	RenderComponent newComponent;
	newComponent.SetProgram(m_program);
	m_renderComponents.push_back(newComponent);
	return &m_renderComponents[m_renderComponents.size() - 1];
}