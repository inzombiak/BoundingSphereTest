#pragma once
#include "RenderComponent.h"

class Circle
{
public:
	//Default constructor
	Circle(){ m_renderComponent = nullptr; };

	//Initializes Circle with given center, radius, color and render component
	Circle(glm::vec3 position, float radius, glm::vec3 color, RenderComponent* renderComp)
	{
		//Set radius and render component
		m_radius = radius;
		m_renderComponent = renderComp;

		//Update center of the circle
		SetCenter(position);

		//Set how to draw this object
		renderComp->SetDrawPrimitive(GL_LINES);

		m_color = color;

		//Create temp vector to pass as a color buffer
		std::vector<glm::vec3> tempColor;
		tempColor.push_back(color);
		renderComp->SetColor(tempColor);
	}

	//Attach a render component to circle
	void SetRenderComponent(RenderComponent* renderComp)
	{
		m_renderComponent = renderComp;

		//Set how to draw this object
		renderComp->SetDrawPrimitive(GL_LINES);

		//Create temp vector to pass as a color buffer
		std::vector<glm::vec3> tempColor;
		tempColor.push_back(m_color);
		renderComp->SetColor(tempColor);

		//Update the vertex buffer
		UpdateRenderComponent();
	}

	//Set the center, will update render component
	void SetCenter(glm::vec3 position);
	//Get center
	glm::vec3 GetCenter() const
	{
		return m_center;
	};

	//Set the radius, will update render component
	void SetRadius(float radius);
	//Get radius
	float GetRadius() const
	{
		return m_radius;
	};

	//Set color of the circle
	void SetColor(glm::vec3 color)
	{
		//Update color
		m_color = color;

		//If circle has a render component, update it
		if (m_renderComponent)
		{
			std::vector<glm::vec3> tempColor;
			tempColor.push_back(color);
			m_renderComponent->SetColor(tempColor);
		}		
	}

	//Returns true if point is in circle or on the perimiter, otherwise returns false
	bool IsInCircle(glm::vec3 point)
	{
		return ((m_center.x - point.x)*(m_center.x - point.x) + (m_center.y - point.y)*(m_center.y - point.y) + (m_center.z - point.z)*(m_center.z - point.z) <= m_radius * m_radius);
	}

private:
	//How many triangles circle should be made up of
	static const int NUMBER_OF_TRIANGLES = 30;

	//Updates the render components color and vertex buffer
	void UpdateRenderComponent();

	//Center of the circle
	glm::vec3 m_center;
	//Color of the circle
	glm::vec3 m_color;
	//Points describing the perimiter of the circle
	std::vector<glm::vec3> m_points;
	//Radius of the circle
	float m_radius;
	//Circles render components
	RenderComponent* m_renderComponent;
};

