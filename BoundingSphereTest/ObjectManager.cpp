#include "ObjectManager.h"
#include "WelzlMBC.h"
#include "HelperFunctions.h"

#include <chrono>

#include <random>

void ObjectManager::Init()
{
	//Seed random engine
	std::random_device rd;
	m_randomMTEngine.seed(rd());
	
	//Initalize render manager
	m_renderManager.Init();
}

void ObjectManager::Clear()
{
	//Clear existing data
	m_renderManager.Clear();
	m_points.clear();
}

void ObjectManager::Draw(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{
	//Draw screen
	m_renderManager.Draw(position, direction, up, m_is3D);
}

void ObjectManager::GenerateRandomPoints2D(int numberOfPoints, glm::vec2 xBounds, glm::vec2 yBounds)
{
	glm::vec3 position;
	glm::vec3 color(0.0f, 0.0f, 1.0f);
	for (int i = 0; i < numberOfPoints; i++)
	{
		position = CalcRandomPosition(xBounds, yBounds, glm::vec2(0));
		Point newPoint(position,color,m_renderManager.CreateRenderComponent());
		m_points.push_back(newPoint);
	}

	m_is3D = false;
}

void ObjectManager::GenerateRandomPoints3D(int numberOfPoints, glm::vec2 xBounds, glm::vec2 yBounds, glm::vec2 zBounds)
{
	glm::vec3 position;
	glm::vec3 color(0.0f, 0.0f, 1.0f);
	for (int i = 0; i < numberOfPoints; i++)
	{
		position = CalcRandomPosition(xBounds, yBounds, zBounds);
		Point newPoint(position, color, m_renderManager.CreateRenderComponent());
		m_points.push_back(newPoint);
	}

	m_is3D = true;
}

glm::vec3 ObjectManager::CalcRandomPosition(glm::vec2 xBounds, glm::vec2 yBounds, glm::vec2 zBounds)
{
	glm::vec3 result;
	//Generate 3 distributuions, one for each axis
	std::uniform_real_distribution<double> distX(xBounds.x, xBounds.y);
	std::uniform_real_distribution<double> distY(yBounds.x, yBounds.y);
	std::uniform_real_distribution<double> distZ(zBounds.x, zBounds.y);

	//Get a random value
	result.x = distX(m_randomMTEngine);
	result.y = distY(m_randomMTEngine);
	result.z = distZ(m_randomMTEngine);

	return result;
}

void ObjectManager::CalculateBoundingShape()
{
	//Initalize calculator. TODO MAKE STATIC
	WelzlMBC welzlCalculator;

	//Call appropriate method based on 2D or 3D
	if (!m_is3D)
	{
		//Get start time
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		//Calculate MBC
		m_boundingCircle = welzlCalculator.CalculateMinBoundingCircle(m_points);
		//Get end time
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		//Calculate difference
		auto diff = t2 - t1;
		//Get duration in seconds
		auto duration = std::chrono::duration<double, std::ratio<1, 1>>(diff).count();

		printf("MBC Time: %lf \n", duration);
		//Attach a render component and set its color
		m_boundingCircle.SetRenderComponent(m_renderManager.CreateRenderComponent());
		m_boundingCircle.SetColor(glm::vec3(0.0f, 0.0f, 0.0f));

		
	}
	else
	{
		//*****SAME AS ABOVE****

		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		m_boundingSphere = welzlCalculator.CalculateMinBoundingSphere(m_points);
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		auto diff =  t2 - t1;

		auto duration = std::chrono::duration<double, std::ratio<1,1>> (diff).count();

		printf("MBS Time: %lf \n", duration);
		m_boundingSphere.SetRenderComponent(m_renderManager.CreateRenderComponent());
		m_boundingCircle.SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
	}
}