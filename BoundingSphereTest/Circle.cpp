#include "Circle.h"

void Circle::SetCenter(glm::vec3 position)
{
	position.z = 0;
	m_center = position;

	std::vector<glm::vec3> vertexBuffer;
	vertexBuffer.push_back(m_center);
	float stepAngle = atan(1) * 8 / NUMBER_OF_TRIANGLES;
	for (int i = 0; i <= NUMBER_OF_TRIANGLES; ++i)
	{
		position.x = m_center.x + (m_radius * cos(stepAngle * i));
		position.y = m_center.y + (m_radius * sin(stepAngle * i));
		vertexBuffer.push_back(position);
	}

	m_renderComponent->SetVertices(vertexBuffer);
}

void Circle::SetRadius(float radius)
{

}