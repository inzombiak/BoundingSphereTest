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
	void Init();
	void Clear();

	void GenerateRandomPoints2D(int numberOfPoints, glm::vec2 xBounds, glm::vec2 yBounds);
	void GenerateRandomPoints3D(int numberOfPoints, glm::vec2 xBounds, glm::vec2 yBounds, glm::vec2 zBounds);

	void CalculateBoundingShape();
	void Draw(glm::vec3 position, glm::vec3 direction, glm::vec3 up);

private:
	bool m_is3D = false;

	glm::vec3 CalcRandomPosition(glm::vec2 xBounds, glm::vec2 yBounds, glm::vec2 zBounds);

	std::mt19937 m_randomMTEngine;
	std::vector<Point> m_points;
	Circle m_boundingCircle;
	Sphere m_boundingSphere;
	RenderManager m_renderManager;
};

