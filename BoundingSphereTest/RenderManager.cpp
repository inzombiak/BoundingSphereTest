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

void RenderManager::Draw(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{
	glUseProgram(m_program);
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(
		position,
		position + direction,
		up
		);
	glm::mat4 model(1.0f);

	glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*glm::vec3 testVert(0.5f, 0.7f, 0.0f);
	std::vector<glm::vec3> testVec;
	testVec.push_back(testVert);
	GLuint testBO;
	glGenBuffers(1, &testBO);
	glBindBuffer(GL_ARRAY_BUFFER,testBO);
	glBufferData(GL_ARRAY_BUFFER, testVec.size() * sizeof(glm::vec3), &testVec[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, testBO);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);*/
	//
	//
	//	GLuint MVPMatID = glGetUniformLocation(m_program, "MVP");
	//	glUniformMatrix4fv(MVPMatID, 1, GL_FALSE, &mvp[0][0]);
	//

	//glDrawArrays(GL_POINTS, 0, 1);
	for (unsigned int i = 0; i < m_renderComponents.size(); ++i)
		m_renderComponents[i].Draw(projection*view*model);

	glutSwapBuffers();
}

RenderComponent* RenderManager::CreateRenderComponent()
{
	RenderComponent newComponent;
	newComponent.SetProgram(m_program);
	m_renderComponents.push_back(newComponent);
	return &m_renderComponents[m_renderComponents.size() - 1];
}