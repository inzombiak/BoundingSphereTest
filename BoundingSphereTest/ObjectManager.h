#pragma once

#include "glm\glm.hpp"
#include <vector>
#include <random>
#include <memory>

#include "Circle.h"
#include "RenderManager.h"
#include "Point.h"

class ObjectManager
{
public:
	void Init();
	void Clear();
	void GenerateRandomPoints(int numberOfPoints, glm::vec2 xBounds, glm::vec2 yBounds, glm::vec2 zBounds);
	void CalculateBoundingCircle();
	void Draw(glm::vec3 position, glm::vec3 direction, glm::vec3 up);

private:
	glm::vec3 CalcRandomPosition(glm::vec2 xBounds, glm::vec2 yBounds, glm::vec2 zBounds);
	Circle CreateCircle(glm::vec3 point1, glm::vec3 point2);
	Circle CreateCircle(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);
	Circle WelzlMBC(int n, std::vector<glm::vec3> boundaryPoints);

	std::mt19937 m_randomMTEngine;
	std::vector<Point> m_points;
	std::shared_ptr<Circle> m_boundingCircle;
	RenderManager m_renderManager;
};

