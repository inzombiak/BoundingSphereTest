#include "WelzlMBC.h"

bool WelzlMBC::SupportSetContains(std::vector<glm::vec3> boundaryPoints, glm::vec3 point)
{
	bool result = false;

	unsigned int size = boundaryPoints.size();
	glm::vec3 testPoint(0.0f);

	double dx = 0, dy = 0, dz = 0;

	//Check all the points in the boundary set. 
	//If the distance between any point in the set and the new points is greater than some small number epsilon, we add it to the set
	for (unsigned int i = 0; i < size; ++i)
	{
		testPoint = boundaryPoints[i];
		dx = point.x - testPoint.x;
		dy = point.y - testPoint.y;
		dz = point.z - testPoint.z;

		float distance = (dx * dx + dy * dy + dz * dz);

		if (distance < EPSILON)
		{
			result = true;
			break;
		}
	}

	return result;
}

Circle WelzlMBC::CalculateMinBoundingCircle(std::vector<Point> points)
{
	std::vector<glm::vec3> boundaryPoints;
	Circle minCircle;

	//Create a circle with center at the first point in the given set and a radius of 0
	minCircle.SetCenter(points[0].GetPosition());
	minCircle.SetRadius(0);
	//Push this point into the support set
	boundaryPoints.push_back(points[0].GetPosition());
	//Start loop from second point
	unsigned int index = 1;
	glm::vec3 pi;
	while (index < points.size())
	{
		//Check if the support set doesn't contain the point processed
		//A point in the set is a point that has triggered an update, ignore it
		pi = points[index].GetPosition();
		if (!SupportSetContains(boundaryPoints, pi))
		{
			//Check if the point is outside the current minimum circle
			if (!minCircle.IsInCircle(pi))
			{
				//Update the minimum circle to contain the point
				//If the new circle was indeed enlarged, ok to start over
				Circle newc = UpdateCircle(boundaryPoints, pi);
				if (newc.GetRadius() > minCircle.GetRadius())
				{
					minCircle = newc;
					index = 0;
					continue;
				}
			}
		}
		index++;
	}

	return minCircle;
}
Circle WelzlMBC::UpdateCircle(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point)
{
	Circle updatedCircle;

	int boundarySize = boundaryPoints.size();
	if (boundarySize == 1)
		updatedCircle = UpdateCircleOne(boundaryPoints, point);
	else if (boundarySize == 2)
		updatedCircle = UpdateCircleTwo(boundaryPoints, point);
	else if (boundarySize == 3)
		updatedCircle = UpdateCircleThree(boundaryPoints, point);

	return updatedCircle;
}
Circle WelzlMBC::UpdateCircleOne(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point)
{
	//If we have one point on the boundary, create a circle from two points
	boundaryPoints.push_back(point);

	return CreateCircle(boundaryPoints[0], point);
}
Circle WelzlMBC::UpdateCircleTwo(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point)
{
	glm::vec3 point0 = boundaryPoints[0], point1 = boundaryPoints[1];

	Circle circles[3];
	float minRad2 = std::numeric_limits<float>::max();
	int index = -1;

	//Create circle from point 0 and p and see if point 1 is within it
	circles[0] = CreateCircle(point0, point);
	if (circles[0].IsInCircle(point1))
	{
		minRad2 = circles[0].GetRadius();
		index = 0;
	}

	//Create circle from point 1 and p and see if point 0 is within it
	circles[1] = CreateCircle(point1, point);
	if (circles[1].GetRadius() < minRad2 && circles[1].IsInCircle(point0))
	{
		minRad2 = circles[1].GetRadius();
		index = 1;
	}

	Circle minc;
	if (index != -1)
	{
		//One of the permutations contains all points, keep it
		minc = circles[index];
		boundaryPoints[1 - index] = point;
	}
	else
	{
		//Enlarge circle to encompass all three points and add p to set
		minc = CreateCircle(point0, point1, point);
		boundaryPoints.push_back(point);
	}
	return minc;
}
Circle WelzlMBC::UpdateCircleThree(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point)
{
	//Init a total of 6 possible circles
	Circle circles[6];
	//Radius of minimum circle
	float minRad2 = std::numeric_limits<float>::max();
	//Index of minimum circle
	int index = -1;

	//Init the points in the permutation tests
	glm::vec3 p0 = boundaryPoints[0];
	glm::vec3 p1 = boundaryPoints[1];
	glm::vec3 p2 = boundaryPoints[2];
	glm::vec3 p3 = point;

	//Construct a circle from p0 and p3 and check if p1 and p2 are inside it
	circles[0] = CreateCircle(p0, p3);
	if (circles[0].IsInCircle(p1) && circles[0].IsInCircle(p2))
	{
		minRad2 = circles[0].GetRadius();
		index = 0;
	}

	//Construct a circle from p1 and p3 and check if p0 and p2 are inside it
	circles[1] = CreateCircle(p1, p3);
	if (circles[1].GetRadius() < minRad2 &&
		circles[1].IsInCircle(p0) && circles[1].IsInCircle(p2))
	{
		minRad2 = circles[1].GetRadius();
		index = 1;
	}

	//Construct a circle from p2 and p3 and check if p0 and p1 are inside it
	circles[2] = CreateCircle(p2, p3);
	if (circles[2].GetRadius() < minRad2 &&
		circles[2].IsInCircle(p0) && circles[2].IsInCircle(p1))
	{
		minRad2 = circles[2].GetRadius();
		index = 2;
	}

	//Construct a circle from p0, p1 and p3 and check if p2 is inside it
	circles[3] = CreateCircle(p0, p1, p3);
	if (circles[3].GetRadius() < minRad2 && circles[3].IsInCircle(p2))
	{
		minRad2 = circles[3].GetRadius();
		index = 3;
	}

	//Construct a circle from p0, p2 and p3 and check if p1 is inside it
	circles[4] = CreateCircle(p0, p2, p3);
	if (circles[4].GetRadius() < minRad2 && circles[4].IsInCircle(p1))
	{
		minRad2 = circles[4].GetRadius();
		index = 4;
	}

	//Construct a circle from p1, p2 and p3 and check if p0 is inside it
	circles[5] = CreateCircle(p1, p2, p3);
	if (circles[5].GetRadius() < minRad2 && circles[5].IsInCircle(p0))
	{
		minRad2 = circles[5].GetRadius();
		index = 5;
	}

	//Get the minimum circle
	Circle minc = circles[index];

	//Update boundary set
	switch (index)
	{
	case 0:
		//Two points, p replaces second point
		boundaryPoints.resize(2);
		boundaryPoints[1] = point;
		break;
	case 1:
		//Two points, p replaces first point
		boundaryPoints.resize(2);
		boundaryPoints[0] = point;
		break;
	case 2:
	{
		//Two points, p3 replaces p1 and p0 replaces p2
		glm::vec3 temp = boundaryPoints[2];
		boundaryPoints.resize(2);
		boundaryPoints[0] = temp;
		boundaryPoints[1] = point;
		break;
	}
	case 3:
		//Three points, p replaces third point
		boundaryPoints[2] = point;
		break;
	case 4:
		//Three points, p replaces second point
		boundaryPoints[1] = point;
		break;
	case 5:
		//Three points, p replaces first point
		boundaryPoints[0] = point;
		break;
	}

	return minc;
}
Circle WelzlMBC::CreateCircle(glm::vec3 point1, glm::vec3 point2)
{
	Circle result;

	double p1x = point1.x;
	double p1y = point1.y;
	double p2x = point2.x;
	double p2y = point2.y;

	//Center is the midpoint of line connecting the 2 points
	double cx = 0.5*(p1x + p2x);
	double cy = 0.5*(p1y + p2y);

	result.SetCenter(glm::vec3(cx, cy, 0));
	result.SetRadius(sqrt((p1x - cx)*(p1x - cx) + (p1y - cy)*(p1y - cy)));
	return result;
}
Circle WelzlMBC::CreateCircle(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3)
{
	//Deatils can be found on Stephen R. Schmitts site - http://www.abecedarical.com/zenosamples/zs_circle3pts.html
	
	Circle result;

	double p1x = point1.x;
	double p1y = point1.y;
	double p2x = point2.x;
	double p2y = point2.y;
	double p3x = point3.x;
	double p3y = point3.y;

	double a = p2x - p1x;
	double b = p2y - p1y;
	double c = p3x - p1x;
	double d = p3y - p1y;
	double e = a * (p2x + p1x) * 0.5 + b * (p2y + p1y) * 0.5;
	double f = c * (p3x + p1x) * 0.5 + d * (p3y + p1y) * 0.5;
	double det = a*d - b*c;

	double cx = (d*e - b*f) / det;
	double cy = (-c*e + a*f) / det;

	result.SetCenter(glm::vec3(cx, cy, 0));
	result.SetRadius(sqrt((p1x - cx)*(p1x - cx) + (p1y - cy)*(p1y - cy)));

	return result;
}

Sphere WelzlMBC::CalculateMinBoundingSphere(std::vector<Point> points)
{
	/*
	Works the same way as 2D
	*/

	std::vector<glm::vec3> boundaryPoints;
	Sphere minSphere;

	minSphere.SetCenter(points[0].GetPosition());
	minSphere.SetRadius(0);

	unsigned int index = 1;
	boundaryPoints.push_back(points[0].GetPosition());
	glm::vec3 pi;
	while (index < points.size())
	{
		pi = points[index].GetPosition();
		if (!SupportSetContains(boundaryPoints, pi))
		{
			if (!minSphere.IsInSphere(pi))
			{
				Sphere newc = UpdateSphere(boundaryPoints, pi);
				if (newc.GetRadius() > minSphere.GetRadius())
				{
					minSphere = newc;
					index = 0;
					continue;
				}
			}
		}
		index++;
	}

	return minSphere;
}
Sphere WelzlMBC::UpdateSphere(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point)
{
	/*
	Same as 2D with one extra case
	*/

	Sphere updatedSphere;

	int boundarySize = boundaryPoints.size();
	if (boundarySize == 1)
		updatedSphere = UpdateSphereOne(boundaryPoints, point);
	else if (boundarySize == 2)
		updatedSphere = UpdateSphereTwo(boundaryPoints, point);
	else if (boundarySize == 3)
		updatedSphere = UpdateSphereThree(boundaryPoints, point);
	else if (boundarySize == 4)
		updatedSphere = UpdateSphereFour(boundaryPoints, point);

	return updatedSphere;

}
Sphere WelzlMBC::UpdateSphereOne(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point)
{
	boundaryPoints.push_back(point);

	return CreateSphere(boundaryPoints[0], point);
}
Sphere WelzlMBC::UpdateSphereTwo(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point)
{
	glm::vec3 point0 = boundaryPoints[0], point1 = boundaryPoints[1];
	
	Sphere spheres[2];
	//Minimum radius
	float minRad2 = std::numeric_limits<float>::max();
	//Index of minimum sphere
	int index = -1;

	//Create sphere from point 0 and p and see if point 1 is within it
	spheres[0] = CreateSphere(point0, point);
	if (spheres[0].IsInSphere(point1))
	{
		minRad2 = spheres[0].GetRadius();
		index = 0;
	}

	//Create sphere from point 1 and p and see if point 0 is within it
	spheres[1] = CreateSphere(point1, point);
	if (spheres[1].GetRadius() < minRad2 && spheres[1].IsInSphere(point0))
	{
		minRad2 = spheres[1].GetRadius();
		index = 1;
	}

	Sphere mins;
	if (index != -1)
	{
		//One of the permutations contains all points, keep it
		mins = spheres[index];
		boundaryPoints[1 - index] = point;
	}
	else
	{
		//Enlarge sphere to encompass three points and add p to set
		mins = CreateSphere(point0, point1, point);
		boundaryPoints.push_back(point);
	}
	return mins;
}
Sphere WelzlMBC::UpdateSphereThree(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point)
{
	//Init a total of 6 possible spheres
	Sphere spheres[6];
	float minRad2 = std::numeric_limits<float>::max();
	int index = -1;

	//Init the points in the permutation tests
	glm::vec3 p0 = boundaryPoints[0];
	glm::vec3 p1 = boundaryPoints[1];
	glm::vec3 p2 = boundaryPoints[2];
	glm::vec3 p3 = point;
	
	//Construct a sphere from p0 and p3 and check if p1 and p2 are inside it
	spheres[0] = CreateSphere(p0, p3);
	if (spheres[0].IsInSphere(p1) && spheres[0].IsInSphere(p2))
	{
		minRad2 = spheres[0].GetRadius();
		index = 0;
	}

	//Construct a sphere from p1 and p3 and check if p0 and p2 are inside it
	spheres[1] = CreateSphere(p1, p3);
	if (spheres[1].GetRadius() < minRad2 &&
		spheres[1].IsInSphere(p0) && spheres[1].IsInSphere(p2))
	{
		minRad2 = spheres[1].GetRadius();
		index = 1;
	}

	//Construct a sphere from p2 and p3 and check if p0 and p1 are inside it
	spheres[2] = CreateSphere(p2, p3);
	if (spheres[2].GetRadius() < minRad2 &&
		spheres[2].IsInSphere(p0) && spheres[2].IsInSphere(p1))
	{
		minRad2 = spheres[2].GetRadius();
		index = 2;
	}

	//Construct a sphere from p0, p1 and p3 and check if p2 is inside it
	spheres[3] = CreateSphere(p0, p1, p3);
	if (spheres[3].GetRadius() < minRad2 && spheres[3].IsInSphere(p2))
	{
		minRad2 = spheres[3].GetRadius();
		index = 3;
	}

	//Construct a sphere from p0, p2 and p3 and check if p1 is inside it
	spheres[4] = CreateSphere(p0, p2, p3);
	if (spheres[4].GetRadius() < minRad2 && spheres[4].IsInSphere(p1))
	{
		minRad2 = spheres[4].GetRadius();
		index = 4;
	}

	//Construct a sphere from p1, p2 and p3 and check if p0 is inside it
	spheres[5] = CreateSphere(p1, p2, p3);
	if (spheres[5].GetRadius() < minRad2 && spheres[5].IsInSphere(p0))
	{
		minRad2 = spheres[5].GetRadius();
		index = 5;
	}

	//Get the minimum sphere
	Sphere mins;
	//If index hasnt change construct a sphere from all 4 points
	if (index == -1)
	{
		mins = CreateSphere(p0, p1, p2, p3);
		boundaryPoints.push_back(point);
		return mins;
	}

	mins = spheres[index];

	//Update set of support
	switch (index)
	{
	case 0:
		//Two points, p replaces second point
		boundaryPoints.resize(2);
		boundaryPoints[1] = point;
		break;
	case 1:
		//Two points, p replaces first point
		boundaryPoints.resize(2);
		boundaryPoints[0] = point;
		break;
	case 2:
	{
		//p3 replaces p1 and p0 replaces p2
		glm::vec3 temp = boundaryPoints[2];
		boundaryPoints.resize(2);
		boundaryPoints[0] = temp;
		boundaryPoints[1] = point;
		break;
	}
	case 3:
		//Three points, p replaces third point
		boundaryPoints[2] = point;
		break;
	case 4:
		//Three points, p replaces second point
		boundaryPoints[1] = point;
		break;
	case 5:
		//Three points, p replaces first point
		boundaryPoints[0] = point;
		break;
	}

	return mins;
}
Sphere WelzlMBC::UpdateSphereFour(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point)
{
	//Init a total of 14 possible spheres
	std::vector<Sphere> spheres;
	spheres.reserve(14);
	float minRad2 = std::numeric_limits<float>::max();
	int index = -1;

	bool pointsAreInSphere = false;
	//Init the points in the permutation tests
	glm::vec3 p0 = boundaryPoints[0];
	glm::vec3 p1 = boundaryPoints[1];
	glm::vec3 p2 = boundaryPoints[2];
	glm::vec3 p3 = boundaryPoints[3];
	glm::vec3 p4 = point;

	//Test 2 point spheres, 4 total
	for (int i = 0; i < 4; ++i)
	{
		pointsAreInSphere = false;
		spheres.push_back(CreateSphere(boundaryPoints[i], p4));
		//If sphere is smaller than previous test if all points are in the sphere
		if (spheres.back().GetRadius() < minRad2)
		{
			pointsAreInSphere = true;
			for (int j = 1; j < 4; ++j)
			{
				if (!spheres.back().IsInSphere(boundaryPoints[(i + j) % 4]))
				{
					pointsAreInSphere = false;
					break;
				}
			}
		}

		//If all points are in the sphere, set it as the new min
		if (pointsAreInSphere)
		{
			minRad2 = spheres.back().GetRadius();
			index = spheres.size()-1;
		}
	}

	//Test 3 point spheres, 6 total
	for (int i = 0; i < 4; ++i)
	{
		for (int k = i + 1; k < 4; ++k)
		{
			pointsAreInSphere = false;
			spheres.push_back(CreateSphere(boundaryPoints[i], boundaryPoints[k], p4));
			if (spheres.back().GetRadius() < minRad2)
			{
				pointsAreInSphere = true;
 				for (int j = 1; j < 4; ++j)
				{
					if (!spheres.back().IsInSphere(boundaryPoints[(i + j) % 4]))
					{
						pointsAreInSphere = false;
						break;
					}
				}
			}

			if (pointsAreInSphere)
			{
				minRad2 = spheres.back().GetRadius();
				index = spheres.size() - 1;
			}
		}
	}

	//Test 4 point sphere, 4 total
	for (int i = 0; i < 4; ++i)
	{
		pointsAreInSphere = false;
		spheres.push_back(CreateSphere(boundaryPoints[i], boundaryPoints[(i + 1) % 4], boundaryPoints[(i + 2) % 4], p4));
		if (spheres.back().GetRadius() < minRad2)
		{
			pointsAreInSphere = true;
			for (int j = 1; j < 4; ++j)
			{
				if (!spheres.back().IsInSphere(boundaryPoints[(i + j) % 4]))
				{
					pointsAreInSphere = false;
					break;
				}
			}
		}

		if (pointsAreInSphere)
		{
			minRad2 = spheres.back().GetRadius();
			index = spheres.size() - 1;
		}
	}

	if (index == -1)
		return Sphere();

	Sphere mins = spheres[index];

	// update set of support
	switch (index)
	{
	case 0:
		//Two points, p replaces second point
		boundaryPoints.resize(2);
		boundaryPoints[1] = point;
		break;
	case 1:
		//Two points, p replaces first point
		boundaryPoints.resize(2);
		boundaryPoints[0] = point;
		break;
	case 2:
	{
		//Two points, p0 replaces p2, p3 replaces p1
		glm::vec3 temp = boundaryPoints[2];
		boundaryPoints.resize(2);
		boundaryPoints[0] = temp;
		boundaryPoints[1] = point;
		break;
	}
	case 3:
	{
		//Two points, p0 replaces p2, p4 replaces p1
		glm::vec3 temp = boundaryPoints[3];
		boundaryPoints.resize(2);
		boundaryPoints[0] = temp;
		boundaryPoints[1] = point;
		break;
	}
	case 4:
		//Three points, p0 replaces p3
		boundaryPoints.resize(3);
		boundaryPoints[2] = point;
		break;
	case 5:
	{
		//Three points, p0 replaces p3, p3 replaces p2 
		glm::vec3 temp = boundaryPoints[2];
		boundaryPoints.resize(3);
		boundaryPoints[1] = temp;
		boundaryPoints[2] = point;
		break;
	}
	case 6:
	{
		//Three points, p0 replaces p3, p4 replaces p2
		glm::vec3 temp = boundaryPoints[3];
		boundaryPoints.resize(3);
		boundaryPoints[1] = temp;
		boundaryPoints[2] = point;
		break;
	}
	case 7:
	{
		//Three points, p0 replaces p3, p3 replaces p2, p2 replaces p1
		glm::vec3 temp1 = boundaryPoints[1];
		glm::vec3 temp2 = boundaryPoints[2];
		boundaryPoints.resize(3);
		boundaryPoints[0] = temp1;
		boundaryPoints[1] = temp2;
		boundaryPoints[2] = point;
		break;
	}
	case 8:
	{
		//Three points, p0 replaces p3, p2 replaces p1, p4 replaces p2
		glm::vec3 temp1 = boundaryPoints[1];
		glm::vec3 temp2 = boundaryPoints[3];
		boundaryPoints.resize(3);
		boundaryPoints[0] = temp1;
		boundaryPoints[1] = temp2;
		boundaryPoints[2] = point;
		break;
	}
	case 9:
	{
		//Three points, p0 replaces p3, p3 replaces p1, p4 replaces p2
		glm::vec3 temp1 = boundaryPoints[2];
		glm::vec3 temp2 = boundaryPoints[3];
		boundaryPoints.resize(3);
		boundaryPoints[0] = temp1;
		boundaryPoints[1] = temp2;
		boundaryPoints[2] = point;
		break;
	}
	case 10:
		//Four points, p0 replaces p4
		boundaryPoints[3] = point;
		break;
	case 11:
		//Four points, p0 replaces p1
		std::rotate(boundaryPoints.begin(), boundaryPoints.begin() + 1,boundaryPoints.end());
		boundaryPoints[3] = point;
		break;
	case 12:
		//Four points, p0 replaces p2
		std::rotate(boundaryPoints.begin(), boundaryPoints.begin() + 2, boundaryPoints.end());
		boundaryPoints[3] = point;
		break;
	case 13:
		//Four points, p0 replaces p3
		std::rotate(boundaryPoints.begin(), boundaryPoints.begin() + 3, boundaryPoints.end());
		boundaryPoints[3] = point;
		break;
	}

	return mins;
}	  		   
Sphere WelzlMBC::CreateSphere(glm::vec3 point1, glm::vec3 point2)
{
	//Same as circle from 2 points, center is in the middle of the 2 points
	Sphere result = Sphere();

	float p1x = point1.x;
	float p1y = point1.y;
	float p1z = point1.z;
	float p2x = point2.x;
	float p2y = point2.y;
	float p2z = point2.z;

	float cx = 0.5*(p1x + p2x);
	float cy = 0.5*(p1y + p2y);
	float cz = 0.5*(p1z + p2z);

	result.SetCenter(glm::vec3(cx, cy, cz));
	result.SetRadius(sqrt((p1x - cx)*(p1x - cx) + (p1y - cy)*(p1y - cy) + (p1z - cz)*(p1z - cz)));

	return result;
}
Sphere WelzlMBC::CreateSphere(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3)
{
	//Formula can be found on Wikipedia - https://en.wikipedia.org/wiki/Circumscribed_circle#Cartesian_coordinates_from_cross-_and_dot-products
	Sphere result = Sphere();

	glm::vec3 vec12 = point1 - point2;
	glm::vec3 vec23 = point2 - point3;
	glm::vec3 vec31 = point3 - point1;
	glm::vec3 vec21 = point2 - point1;
	glm::vec3 vec32 = point3 - point2;
	glm::vec3 vec13 = point1 - point3;

	float d12 = glm::length(vec12);
	float d23 = glm::length(vec23);
	float d31 = glm::length(vec31);

	float normalLength = glm::length(glm::cross(vec12, vec23));
	float normalLengthSqr = normalLength * normalLength;
	float radius = (d12*d23*d31) / (2 * normalLength);

	float a = (d23 * d23) * (glm::dot(vec12, vec13)) / (2 * normalLengthSqr);
	float b = (d31 * d31) * (glm::dot(vec21, vec23)) / (2 * normalLengthSqr);
	float c = (d12 * d12) * (glm::dot(vec31, vec32)) / (2 * normalLengthSqr);

	result.SetCenter(a * point1 + b * point2 + c * point3);
	result.SetRadius(radius);
	return result;
}
Sphere WelzlMBC::CreateSphere(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec3 point4)
{
	//Once again a thank you to Stephen R. Schmitt - http://www.abecedarical.com/zenosamples/zs_sphere4pts.html

	Sphere result = Sphere();

	glm::mat4x4 matrix (0.0f);
	float radius = 0, m11 = 0, m12 = 0, m13 = 0, m14 = 0, m15 = 0;
	glm::vec3 center(0.0f);
	int i = 0;

	std::vector<glm::vec3> points = { point1, point2, point3, point4 };

	for (i = 0; i < 4; ++i)
	{
		matrix[i][0] = points[i].x;
		matrix[i][1] = points[i].y;
		matrix[i][2] = points[i].z;
		matrix[i][3] = 1;
	}
	m11 = glm::determinant(matrix);

	for (i = 0; i < 4; ++i)
	{
		matrix[i][0] = points[i].x * points[i].x + points[i].y * points[i].y + points[i].z * points[i].z;
		matrix[i][1] = points[i].y;
		matrix[i][2] = points[i].z;
		matrix[i][3] = 1;
	}
	m12 = glm::determinant(matrix);

	for (i = 0; i < 4; ++i)
	{
		matrix[i][0] = points[i].x * points[i].x + points[i].y * points[i].y + points[i].z * points[i].z;
		matrix[i][1] = points[i].x;
		matrix[i][2] = points[i].z;
		matrix[i][3] = 1;
	}
	m13 = glm::determinant(matrix);

	for (i = 0; i < 4; ++i)
	{
		matrix[i][0] = points[i].x * points[i].x + points[i].y * points[i].y + points[i].z * points[i].z;
		matrix[i][1] = points[i].x;
		matrix[i][2] = points[i].y;
		matrix[i][3] = 1;
	}
	m14 = glm::determinant(matrix);

	for (i = 0; i < 4; ++i)
	{
		matrix[i][0] = points[i].x * points[i].x + points[i].y * points[i].y + points[i].z * points[i].z;
		matrix[i][1] = points[i].x;
		matrix[i][2] = points[i].y;
		matrix[i][3] = points[i].z;
	}
	m15 = glm::determinant(matrix);

	if (m11 == 0)
	{
		radius = 0;
		center = point1;
	}
	else
	{
		center.x = 0.5 * m12 / m11;
		center.y = -0.5 * m13 / m11;
		center.z = 0.5 * m14 / m11;
		radius = sqrt(center.x * center.x + center.y * center.y + center.z * center.z - m15 / m11);
	}				  
	

	result.SetRadius(radius);
	result.SetCenter(center);

	return result;
}

const double WelzlMBC::EPSILON = 0.0000001;