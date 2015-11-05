#include "ObjectManager.h"
#include "WelzlMBC.h"
#include "HelperFunctions.h"

#include <random>

void ObjectManager::Init()
{
	std::random_device rd;
	m_randomMTEngine.seed(rd());
	
	m_renderManager.Init();
}

void ObjectManager::Clear()
{
	m_renderManager.Clear();
	m_points.clear();
}

void ObjectManager::Draw(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{
	m_renderManager.Draw(position, direction, up, m_is3D);
}

void ObjectManager::GenerateRandomPoints2D(int numberOfPoints, glm::vec2 xBounds, glm::vec2 yBounds)
{
	glm::vec3 position;
	glm::vec3 color(0.0f, 0.0f, 1.0f);
	for (unsigned int i = 0; i < numberOfPoints; i++)
	{
		position = CalcRandomPosition(xBounds, yBounds, glm::vec2(0));
		Point newPoint(position,color,m_renderManager.CreateRenderComponent());
		m_points.push_back(newPoint);
	}

	m_is3D = false;
}

void ObjectManager::GenerateRandomPoints3D(int numberOfPoints, glm::vec2 xBounds, glm::vec2 yBounds, glm::vec2 zBounds)
{
	//glm::vec3 position;
	glm::vec3 color(0.0f, 0.0f, 1.0f);
	//for (unsigned int i = 0; i < numberOfPoints; i++)
	//{
	//	position = CalcRandomPosition(xBounds, yBounds, zBounds);
	//	Point newPoint(position, color, m_renderManager.CreateRenderComponent());
	//	m_points.push_back(newPoint);
	//}

	glm::vec3 point1(0.5, 0, 0), point2(-0.5, 0, 0), point3(0.5, 0.5, 0), point4(-0.5, 0.5, 0);

	Point test1(point1, color, m_renderManager.CreateRenderComponent()), test2(point2, color, m_renderManager.CreateRenderComponent()), test3(point3, color, m_renderManager.CreateRenderComponent()), test4(point4, color, m_renderManager.CreateRenderComponent());
	WelzlMBC testMBC;
	//Sphere test2Points = testMBC.CreateSphere(test1.GetPosition(), test2.GetPosition());
	//test2Points.SetRenderComponent(m_renderManager.CreateRenderComponent());

	Sphere test4Points = testMBC.CreateSphere(test1.GetPosition(), test2.GetPosition(), test3.GetPosition(), test4.GetPosition());
	//Circle test3PointsCircle = testMBC.CreateCircle(test1.GetPosition(), test2.GetPosition(), test3.GetPosition());
	//test3PointsCircle.SetRenderComponent(m_renderManager.CreateRenderComponent());
	test4Points.SetRenderComponent(m_renderManager.CreateRenderComponent());

	m_is3D = true;
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

void ObjectManager::CalculateBoundingShape()
{
	WelzlMBC welzlCalculator;

	if (!m_is3D)
	{
		m_boundingCircle = welzlCalculator.CalculateMinBoundingCircle(m_points);
		m_boundingCircle.SetRenderComponent(m_renderManager.CreateRenderComponent());
		m_boundingCircle.SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
	}
	else
	{
		m_boundingSphere = welzlCalculator.CalculateMinBoundingSphere(m_points);
		m_boundingSphere.SetRenderComponent(m_renderManager.CreateRenderComponent());
		m_boundingCircle.SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
	}
}