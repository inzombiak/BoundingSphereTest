#include "RenderManager.h"
#define FREEGLUT_STATIC
#define GLEW_STATIC
#include <gl\glew.h>
#include <gl\freeglut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "RenderComponent.h"

void RenderManager::Init()
{

}

void RenderManager::Draw(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{

	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(
		position,
		position + direction,
		up
		);

	glClearColor(1.0f, 1.0f, 0.3f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}

RenderComponent* RenderManager::CreateRenderComponent()
{
	RenderComponent newComponent;
	m_renderComponents.push_back(newComponent);
	return &m_renderComponents[m_renderComponents.size() - 1];
}