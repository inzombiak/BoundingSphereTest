#pragma once

#include "glm\glm.hpp"
#include <vector>

#include "Circle.h"
#include "RenderManager.h"

class RenderManager;
class Point;
class ObjectManager
{
public:
	void Init();

	void GenerateRandomPoints(int numberOfPoints, glm::vec2 xBounds, glm::vec2 yBounds, glm::vec2 zBounds);
	void CalculateBoundingCircle();
	void Draw(glm::vec3 position, glm::vec3 direction, glm::vec3 up);

private:
	glm::vec3 CalcRandomPosition(glm::vec2 xBounds, glm::vec2 yBounds, glm::vec2 zBounds);
	std::vector<Point> m_points;
	Circle m_boundingCircle;
	RenderManager m_renderManager;
};

