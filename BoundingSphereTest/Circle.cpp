#include "Circle.h"

void Circle::SetCenter(glm::vec3 position)
{
	m_center = position;
	//If circle has a render component, update it
	if (m_renderComponent != 0)
		UpdateRenderComponent();
	
}

void Circle::SetRadius(float radius)
{
	m_radius = radius;
	//If circle has a render component, update it
	if (m_renderComponent)
		UpdateRenderComponent();
}

void Circle::UpdateRenderComponent()
{
	glm::vec3 position;
	std::vector<glm::vec3> vertexBuffer;

	//Calculate the step of the angle, more tirnagles means a more "realistic" circle
	float stepAngle = atan(1) * 8 / NUMBER_OF_TRIANGLES;

	//Calculate first point
	position.x = m_center.x + (m_radius * cos(stepAngle * 0));
	position.y = m_center.y + (m_radius * sin(stepAngle * 0));

	//We push each vertex twice because we use GL_LINE to draw
	for (int i = 1; i <= NUMBER_OF_TRIANGLES; ++i)
	{
		vertexBuffer.push_back(position);
		position.x = m_center.x + (m_radius * cos(stepAngle * i));
		position.y = m_center.y + (m_radius * sin(stepAngle * i));
		vertexBuffer.push_back(position);
	}

	//Make sure we still have a render component and update it
	if (m_renderComponent)
		m_renderComponent->SetVertices(vertexBuffer);
}