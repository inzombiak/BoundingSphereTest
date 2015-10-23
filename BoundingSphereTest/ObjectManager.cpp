#include "ObjectManager.h"

#include "Point.h"

void ObjectManager::Init()
{
	m_renderManager.Init();
}

void ObjectManager::GenerateRandomPoints(int numberOfPoints, glm::vec2 xBounds, glm::vec2 yBounds, glm::vec2 zBounds)
{
	glm::vec3 position;
	glm::vec3 color(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < numberOfPoints; i++)
	{
		position = CalcRandomPosition(xBounds, yBounds, zBounds);
		Point newPoint(position,color,m_renderManager.CreateRenderComponent());
		m_points.push_back(newPoint);
	}
}
void ObjectManager::CalculateBoundingCircle()
{

}
void ObjectManager::Draw(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{
	m_renderManager.Draw(position, direction, up);
}

glm::vec3 ObjectManager::CalcRandomPosition(glm::vec2 xBounds, glm::vec2 yBounds, glm::vec2 zBounds)
{
	glm::vec3 result;
	float random = (float)rand() / (float)RAND_MAX;

	float xDiff = xBounds.y - xBounds.x;
	float yDiff = yBounds.y - yBounds.x;
	float zDiff = zBounds.y - zBounds.x;

	result.x = xBounds.x + random*xDiff;
	result.y = yBounds.x + random*yDiff;
	result.z = zBounds.x + random*zDiff;

	return result;
}