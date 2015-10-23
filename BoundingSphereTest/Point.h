#pragma once
#include "glm\glm.hpp"

class RenderComponent;
class Point
{
public:
	Point(glm::vec3 position, glm::vec3 color, RenderComponent* renderComp)
	{
		m_position = position;
		m_color = color;

		m_renderComponent = renderComp;
		renderComp->SetVertices(position);
		renderComp->SetColor(color);
	}
	
	void SetPosition(glm::vec3 position)
	{
		m_position = position;
		m_renderComponent->SetVertices(position);
	}
	glm::vec3 GetPosition() const
	{
		return m_position;
	};

private:
	glm::vec3 m_position;
	glm::vec3 m_color;
	RenderComponent* m_renderComponent;
};

