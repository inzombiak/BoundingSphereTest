#pragma once
#include "RenderComponent.h"

class Circle
{
public:
	Circle(glm::vec3 position, float radius, RenderComponent* renderComp)
	{
		m_radius = radius;
		m_renderComponent = renderComp;
		SetCenter(position);

		renderComp->SetDrawPrimitive(GL_TRIANGLE_FAN);
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

private:
	static const int NUMBER_OF_TRIANGLES = 30;

	glm::vec3 m_center;
	std::vector<glm::vec3> m_points;
	float m_radius;
	RenderComponent* m_renderComponent;
};

