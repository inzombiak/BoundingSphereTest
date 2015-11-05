#pragma once
#include "RenderComponent.h"
#include <glm\glm.hpp>

class Sphere
{
public:
	Sphere(){ m_renderComponent = nullptr; };

	Sphere(glm::vec3 position, float radius, glm::vec3 color, RenderComponent* renderComp)
	{
		m_radius = radius;
		m_renderComponent = renderComp;
		SetCenter(position);

		m_color = color;
		std::vector<glm::vec3> tempColor;
		tempColor.push_back(color);
		renderComp->SetColor(tempColor);
		renderComp->SetDrawPrimitive(GL_TRIANGLES);
	}

	void SetRenderComponent(RenderComponent* renderComp)
	{
		std::vector<glm::vec3> tempColor;
		tempColor.push_back(m_color);
		renderComp->SetColor(tempColor);
		m_renderComponent = renderComp;
		renderComp->SetDrawPrimitive(GL_TRIANGLES);
		UpdateRenderComponent();
	}

	void SetCenter(glm::vec3 position);
	glm::vec3 GetCenter() const
	{
		return m_center;
	};
	
	void SetColor(glm::vec3 color)
	{
		m_color = color;
		std::vector<glm::vec3> tempColor;
		tempColor.push_back(color);
		m_renderComponent->SetColor(tempColor);
	}

	void SetRadius(float radius);
	float GetRadius() const
	{
		return m_radius;
	};

	bool IsInSphere(glm::vec3 point)
	{
		return ((m_center.x - point.x)*(m_center.x - point.x) + (m_center.y - point.y)*(m_center.y - point.y) + (m_center.z - point.z)*(m_center.z - point.z) <= m_radius * m_radius);
	}

private:
	static const int NUMBER_OF_SPLITS = 4;

	void UpdateRenderComponent();

	glm::vec3 m_center;
	glm::vec3 m_color;
	std::vector<glm::vec3> m_points;
	float m_radius;
	RenderComponent* m_renderComponent;	
};

