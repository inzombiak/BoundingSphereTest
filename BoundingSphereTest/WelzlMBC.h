#pragma once

#include "Circle.h"
#include "Sphere.h"
#include "Point.h"

#include <vector>
#include <glm\glm.hpp>

/*
Welzl MBC/MBS solver. The 2D and 3D funciton are separate, 
but I'm sure I could merge them with some better OOP, but that was not the purpose of this test
Implementation is based on on the Welzl article from gamedev.net - http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/welzl-r2484
*/

class WelzlMBC
{
public:
	//Calculate 2D Welzl (Minimum Bounding Circle)
	Circle CalculateMinBoundingCircle(std::vector<Point> points);
	//Calculate 3D Welzl (Minimum Bounding Sphere)
	Sphere CalculateMinBoundingSphere(std::vector<Point> points);

private:
	//A small static value used for floating point error correction
	static const double EPSILON;
	//Checks if the given support set contains the point
	bool SupportSetContains(std::vector<glm::vec3> boundaryPoints, glm::vec3 point);
	//Updates circle based on number of boundary points
	Circle UpdateCircle(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point);
	//Update for single boundary point
	Circle UpdateCircleOne(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point);
	//Update for two boundary points
	Circle UpdateCircleTwo(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point);
	//Update for three boundary points
	Circle UpdateCircleThree(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point);
	//Creates circle from 2 points
	Circle CreateCircle(glm::vec3 point1, glm::vec3 point2);
	//Creates circle from 3 points
	Circle CreateCircle(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);

	//Following functions are the same as for the 2D case, with 1 extra case since we can create a sphere form 4 non coplanar points.
	Sphere UpdateSphere(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point);
	Sphere UpdateSphereOne(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point);
	Sphere UpdateSphereTwo(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point);
	Sphere UpdateSphereThree(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point);
	Sphere UpdateSphereFour(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point);
	Sphere CreateSphere(glm::vec3 point1, glm::vec3 point2);
	Sphere CreateSphere(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);
	Sphere CreateSphere(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec3 point4);
};

