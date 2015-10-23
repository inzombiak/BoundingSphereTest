#pragma once
#include <vector>

class RenderComponent;
class RenderManager
{
public:
	void Init();
	void Draw(glm::vec3 position, glm::vec3 direction, glm::vec3 up);
	RenderComponent* CreateRenderComponent();

private:
	std::vector<RenderComponent> m_renderComponents;
};

