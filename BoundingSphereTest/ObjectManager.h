#pragma once

#include "glm\glm.hpp"
#include <vector>
#include <random>
#include <memory>

#include "Circle.h"
#include "Sphere.h"
#include "RenderManager.h"
#include "Point.h"

class ObjectManager
{
public:

	//Initalize object manager
	void Init();

	//Clear object manager and render manager
	void Clear();

	//Generate random points in 2D within given bounds
	void GenerateRandomPoints2D(int numberOfPoints, glm::vec2 xBounds, glm::vec2 yBounds);

	//Generate random points in 3D within given bounds
	void GenerateRandomPoints3D(int numberOfPoints, glm::vec2 xBounds, glm::vec2 yBounds, glm::vec2 zBounds);

	//Calculates the MBC/MBS
	void CalculateBoundingShape();

	//Draw the screen
	void Draw(glm::vec3 position, glm::vec3 direction, glm::vec3 up);

private:
	//Keeps track of draw type
	bool m_is3D = false;

	//Calculates a random position within given bounds
	glm::vec3 CalcRandomPosition(glm::vec2 xBounds, glm::vec2 yBounds, glm::vec2 zBounds);

	//Random engine
	std::mt19937 m_randomMTEngine;
	//vector of points
	std::vector<Point> m_points;
	
	//Bounding shapes
	Sphere m_boundingSphere;
	Circle m_boundingCircle;

	//Render manager
	RenderManager m_renderManager;
};

