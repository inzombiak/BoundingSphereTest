#pragma once
#include "RenderComponent.h"

class Circle
{
public:
	Circle(){ m_renderComponent = nullptr; };

	Circle(glm::vec3 position, float radius, RenderComponent* renderComp)
	{
		m_radius = radius;
		m_renderComponent = renderComp;
		SetCenter(position);

		renderComp->SetDrawPrimitive(GL_TRIANGLE_FAN);
	}

	void SetRenderComponent(RenderComponent* renderComp)
	{
		m_renderComponent = renderComp;
		renderComp->SetDrawPrimitive(GL_TRIANGLE_FAN);
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

	bool IsInCircle(glm::vec3 point)
	{
		return ((m_center.x - point.x)*(m_center.x - point.x) + (m_center.y - point.y)*(m_center.y - point.y) <= m_radius * m_radius);
	}

private:
	static const int NUMBER_OF_TRIANGLES = 30;

	void UpdateRenderComponent();

	glm::vec3 m_center;
	std::vector<glm::vec3> m_points;
	float m_radius;
	RenderComponent* m_renderComponent;
};

