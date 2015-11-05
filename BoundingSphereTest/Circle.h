#pragma once
#include "RenderComponent.h"

class Circle
{
public:
	Circle(){ m_renderComponent = nullptr; };

	Circle(glm::vec3 position, float radius, glm::vec3 color, RenderComponent* renderComp)
	{
		m_radius = radius;
		m_renderComponent = renderComp;
		SetCenter(position);

		renderComp->SetDrawPrimitive(GL_LINES);
		m_color = color;
		std::vector<glm::vec3> tempColor;
		tempColor.push_back(color);
		renderComp->SetColor(tempColor);
	}

	void SetRenderComponent(RenderComponent* renderComp)
	{
		m_renderComponent = renderComp;
		renderComp->SetDrawPrimitive(GL_LINES);
		std::vector<glm::vec3> tempColor;
		tempColor.push_back(m_color);
		renderComp->SetColor(tempColor);
		UpdateRenderComponent();
	}

	void SetCenter(glm::vec3 position);
	glm::vec3 GetCenter() const
	{
		return m_center;
	};

	void SetRadius(float radius);
	float GetRadius() const
	{
		return m_radius;
	};

	void SetColor(glm::vec3 color)
	{
		m_color = color;
		std::vector<glm::vec3> tempColor;
		tempColor.push_back(color);
		if (m_renderComponent)
			m_renderComponent->SetColor(tempColor);
	}

	bool IsInCircle(glm::vec3 point)
	{
		return ((m_center.x - point.x)*(m_center.x - point.x) + (m_center.y - point.y)*(m_center.y - point.y) + (m_center.z - point.z)*(m_center.z - point.z) <= m_radius * m_radius);
	}

private:
	static const int NUMBER_OF_TRIANGLES = 30;

	void UpdateRenderComponent();

	glm::vec3 m_center;
	glm::vec3 m_color;
	std::vector<glm::vec3> m_points;
	float m_radius;
	RenderComponent* m_renderComponent;
};

