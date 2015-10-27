#pragma once
#include <vector>

#include "RenderComponent.h"

class RenderComponent;
class RenderManager
{
public:
	void Init();
	void Draw(glm::vec3 position, glm::vec3 direction, glm::vec3 up);
	RenderComponent* CreateRenderComponent();

private:
	GLuint m_program;
	GLuint m_vertexArrayObject;
	std::vector<RenderComponent> m_renderComponents;
};

