#pragma once
#include "RenderComponent.h"
#include <glm\glm.hpp>
#include "glm\gtx\norm.hpp"

class Sphere
{
public:
	Sphere(){ m_renderComponent = nullptr; };

	//Creates a sphere given the center, radius, color and a render component
	Sphere(glm::vec3 position, float radius, glm::vec3 color, RenderComponent* renderComp)
	{
		m_radius = radius;
		m_renderComponent = renderComp;
		SetCenter(position);

		m_color = color;
		//Temporary vector to hold color to be passed to render component
		std::vector<glm::vec3> tempColor;
		tempColor.push_back(color);
		renderComp->SetColor(tempColor);

		//Set draw type
		renderComp->SetDrawPrimitive(GL_TRIANGLES);
	}
	//Set render component
	void SetRenderComponent(RenderComponent* renderComp)
	{
		m_renderComponent = renderComp;
		m_renderComponent->SetDrawPrimitive(GL_TRIANGLES);

		//Update render components vertex buffer
		UpdateRenderComponent();
	}
	//Set center, updates vertex buffer
	void SetCenter(glm::vec3 position);
	//Get center
	glm::vec3 GetCenter() const
	{
		return m_center;
	};
	//Set color
	void SetColor(glm::vec3 color)
	{
		m_color = color;
		if (m_renderComponent)
		{
			std::vector<glm::vec3> tempColor;
			tempColor.push_back(color);
			m_renderComponent->SetColor(tempColor);
		}
			
	}
	//Set radius, updates vertex buffer
	void SetRadius(float radius);
	//Get radius
	float GetRadius() const
	{
		return m_radius;
	};
	//Returns true if point is in sphere or on its perimeter, otherwise false
	bool IsInSphere(glm::vec3 point)
	{
		double distance = glm::distance2(m_center, point);
		return (distance - m_radius*m_radius) < 0.0001;
	}

private:
	//Sphere is created by repeatedly dividing the faces of a octahedron
	static const int NUMBER_OF_SPLITS = 4;
	//Update the render components vertex buffer, recalculates sphere!
	void UpdateRenderComponent();
	//Center of the sphere
	glm::vec3 m_center;
	//Color of the sphere
	glm::vec3 m_color;
	//Points that make up the sphere
	std::vector<glm::vec3> m_points;
	//Radius of the sphere
	float m_radius;
	//Render component
	RenderComponent* m_renderComponent;	
};

