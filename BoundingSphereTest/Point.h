#pragma once
#include "glm\glm.hpp"
#include "RenderComponent.h"

class Point
{
public:
	Point(glm::vec3 position, glm::vec3 color, RenderComponent* renderComp)
	{
		m_position = position;
		m_color = color;
		m_renderComponent = renderComp;
		SetPosition(m_position);

		std::vector<glm::vec3> tempColor;
		tempColor.push_back(color);
		renderComp->SetColor(tempColor);

		renderComp->SetDrawPrimitive(GL_POINTS);
	}
	
	void SetPosition(glm::vec3 position)
	{
		std::vector<glm::vec3> tempVert;
		tempVert.push_back(position);
		m_renderComponent->SetVertices(tempVert);
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

