#pragma once
#include "RenderComponent.h"
#include <glm\glm.hpp>
#include "glm\gtx\norm.hpp"

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
		double distance = glm::distance2(m_center, point);
		return (distance - m_radius*m_radius) < 0.0001;
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

