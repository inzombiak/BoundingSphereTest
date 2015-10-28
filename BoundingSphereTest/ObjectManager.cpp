#include "ObjectManager.h"

#include <random>

void ObjectManager::Init()
{
	std::random_device rd;
	m_randomMTEngine.seed(rd());
	
	m_renderManager.Init();
}

void ObjectManager::GenerateRandomPoints(int numberOfPoints, glm::vec2 xBounds, glm::vec2 yBounds, glm::vec2 zBounds)
{
	glm::vec3 position;
	glm::vec3 color(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < numberOfPoints; i++)
	{
		position = CalcRandomPosition(xBounds, yBounds, zBounds);
		position.z = 0.0f;
		Point newPoint(position,color,m_renderManager.CreateRenderComponent());
		m_points.push_back(newPoint);
	}

	Circle test(m_points[1].GetPosition(), 0.5, m_renderManager.CreateRenderComponent());

	//position.x = 0.5f;
	//position.y = 0.0f;
	//position.z = 0.0f;
	//Point newPoint1(position, color, m_renderManager.CreateRenderComponent());
	//position.x = 0.0f;
	//position.y = 0.5f;
	//Point newPoint2(position, color, m_renderManager.CreateRenderComponent());
	//position.x = -0.5f;
	//position.y = 0.0f;
	//Point newPoint3(position, color, m_renderManager.CreateRenderComponent());
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
	std::uniform_real_distribution<double> distX(xBounds.x, xBounds.y);
	std::uniform_real_distribution<double> distY(yBounds.x, yBounds.y);
	std::uniform_real_distribution<double> distZ(zBounds.x, zBounds.y);

	result.x = distX(m_randomMTEngine);
	result.y = distY(m_randomMTEngine);
	result.z = distZ(m_randomMTEngine);

	return result;
}

void ObjectManager::CreateCircle(glm::vec3 center, float radius)
{

}