#pragma once
#include "glm\glm.hpp"
#include "RenderComponent.h"

class Point
{
public:

	//Initialize point with position, color and render component
	Point(glm::vec3 position, glm::vec3 color, RenderComponent* renderComp)
	{
		//Set color and render component
		m_color = color;
		m_renderComponent = renderComp;
		//Set position, function also updates the render component
		SetPosition(position);

		//Create temp color buffer and give to render component
		std::vector<glm::vec3> tempColor;
		tempColor.push_back(color);
		renderComp->SetColor(tempColor);

		//Set how this object should be drawn
		renderComp->SetDrawPrimitive(GL_POINTS);
	}
	
	//Set the position of the point
	void SetPosition(glm::vec3 position)
	{
		m_position = position;
		//Update render component's vertex buffer
		std::vector<glm::vec3> tempVert;
		tempVert.push_back(position);
		m_renderComponent->SetVertices(tempVert);
	}
	//Get the position of the point
	glm::vec3 GetPosition() const
	{
		return m_position;
	};

private:
	//Position of the point
	glm::vec3 m_position;
	//Color of the point
	glm::vec3 m_color;
	//Render component of the point
	RenderComponent* m_renderComponent;
};

