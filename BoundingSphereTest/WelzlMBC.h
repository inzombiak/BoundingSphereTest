#pragma once

#include "Circle.h"
#include "Sphere.h"
#include "Point.h"

#include <vector>
#include <glm\glm.hpp>

class WelzlMBC
{
public:
	Circle CalculateMinBoundingCircle(std::vector<Point> points);
	Sphere CalculateMinBoundingSphere(std::vector<Point> points);
	Sphere CreateSphere(glm::vec3 point1, glm::vec3 point2);
	Sphere CreateSphere(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);
	Circle CreateCircle(glm::vec3 point1, glm::vec3 point2);
	Circle CreateCircle(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);
	Sphere CreateSphere(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec3 point4);
private:
	static const double EPSILON;

	bool SupportSetContains(std::vector<glm::vec3> boundaryPoints, glm::vec3 point);
	Circle UpdateCircle(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point);
	Circle UpdateCircleOne(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point);
	Circle UpdateCircleTwo(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point);
	Circle UpdateCircleThree(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point);

	Sphere UpdateSphere(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point);
	Sphere UpdateSphereOne(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point);
	Sphere UpdateSphereTwo(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point);
	Sphere UpdateSphereThree(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point);
	Sphere UpdateSphereFour(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point);



};

