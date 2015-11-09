#pragma once
#include <vector>

#include "RenderComponent.h"

class RenderComponent;
class RenderManager
{
public:
	//Initalize render manager
	void Init();
	//Clear existing render components
	void Clear();
	//Draw screen
	void Draw(glm::vec3 position, glm::vec3 direction, glm::vec3 up, bool is3D);
	//Create a render component
	RenderComponent* CreateRenderComponent();

private:
	//Shader program to use for drawing
	GLuint m_program;
	//Vector of render components
	std::vector<RenderComponent> m_renderComponents;
};

