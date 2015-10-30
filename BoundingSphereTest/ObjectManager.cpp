#include "ObjectManager.h"

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
	m_renderManager.Draw(position, direction, up);
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

void ObjectManager::CalculateBoundingCircle()
{
	std::vector<glm::vec3> boundaryPoints;
	int pos;
	std::uniform_int_distribution<int> dist(0, m_points.size()-1);
	for (int i = 0; i < m_points.size(); i++)
	{
		pos = dist(m_randomMTEngine);
		//std::swap(m_points[i], m_points[pos]);
	}
	m_boundingCircle.SetCenter(m_points[0].GetPosition());
	m_boundingCircle.SetRadius(0);
	// inital condition - circle with one point
	int index = 1;
	boundaryPoints.push_back(m_points[0].GetPosition());
	glm::vec3 pi;
	while (index < m_points.size())
	{
		// check if the support set doesn't contain the point processed
		// a point in the set is a point that has triggered an update, ignore it
		pi = m_points[index].GetPosition();
		if (!SupportSetContains(boundaryPoints, pi))
		{
			// check if the point is outside the current minimum circle
			if (!m_boundingCircle.IsInCircle(pi))
			{
				// update the minimum circle to contain the point
				// if the new circle was indeed enlarged, ok to start over
				Circle newc = Update(boundaryPoints, pi);
				if (newc.GetRadius() > m_boundingCircle.GetRadius())
				{
					m_boundingCircle = newc;
					index = 0;
					continue; // restart algorithm
				}
			}
		}
		index++;
	}

	// sqrt the circle's radius
	//m_boundingCircle.SetRadius(sqrt(m_boundingCircle.GetRadius()));

	//printf("Welzl Start\n");
	//m_boundingCircle.reset(WelzlMBC(m_points.size(), boundaryPoints));
	//printf("Welzl End\n");
	m_boundingCircle.SetRenderComponent(m_renderManager.CreateRenderComponent());

	printf("Radius is %f, ", m_boundingCircle.GetRadius());
	printf("Center is %f \n", m_boundingCircle.GetCenter().x);

}

bool ObjectManager::SupportSetContains(std::vector<glm::vec3> boundaryPoints, glm::vec3 point)
{
	bool result = false;

	unsigned int size = boundaryPoints.size();
	glm::vec3 testPoint;

	double dx, dy;

	for (unsigned int i = 0; i < size; ++i)
	{
		testPoint = boundaryPoints[i];
		dx = point.x - testPoint.x;
		dy = point.y - testPoint.y;
		
		float distance = (dx * dx + dy * dy);

		if (distance < EPSILON)
		{
			result = true;
			break;
		}
	}

	return result;
}

Circle ObjectManager::Update(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point)
{
	Circle updatedCircle;

	int boundarySize = boundaryPoints.size();
	if (boundarySize == 1)
		updatedCircle = UpdateOne(boundaryPoints, point);
	else if (boundarySize == 2)
		updatedCircle = UpdateTwo(boundaryPoints, point);
	else if (boundarySize == 3)
		updatedCircle = UpdateThree(boundaryPoints, point);

	return updatedCircle;
}

Circle ObjectManager::UpdateOne(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point)
{
	boundaryPoints.push_back(point);

	return CreateCircle(boundaryPoints[0], point);
}
Circle ObjectManager::UpdateTwo(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point)
{
	glm::vec3 point0 = boundaryPoints[0], point1 = boundaryPoints[1];
	
	Circle circles[3];
	float minRad2 = std::numeric_limits<float>::max();
	int index = -1;

	// create circle from point 0 and p and see if point 1 is within it
	circles[0] = CreateCircle(point0, point);
	if (circles[0].IsInCircle(point1))
	{
		// keep it
		minRad2 = circles[0].GetRadius();
		index = 0;
	}

	// create circle from point 1 and p and see if point 0 is within it
	circles[1] = CreateCircle(point1, point);
	if (circles[1].GetRadius() < minRad2 && circles[1].IsInCircle(point0))
	{
		// keep it
		minRad2 = circles[1].GetRadius();
		index = 1;
	}

	Circle minc;
	if (index != -1)
	{
		// one of the permutations contains all points, keep it
		minc = circles[index];
		boundaryPoints[1 - index] = point;
	}
	else
	{
		// enlarge circle to encompass three points and add p to set
		minc = CreateCircle(point0, point1, point);
		boundaryPoints.push_back(point);
	}
	return minc;
}
Circle ObjectManager::UpdateThree(std::vector<glm::vec3>& boundaryPoints, glm::vec3 point)
{
	// init a total of 6 possible circles
	Circle circles[6];
	float minRad2 = std::numeric_limits<float>::max();
	int index = -1;

	// init the points in the permutation tests
	glm::vec3 p0 = boundaryPoints[0];
	glm::vec3 p1 = boundaryPoints[1];
	glm::vec3 p2 = boundaryPoints[2];
	glm::vec3 p3 = point;

	// construct a circle from p0 and p3 and check if p1 and p2 are inside it
	circles[0] = CreateCircle(p0, p3);
	if (circles[0].IsInCircle(p1) && circles[0].IsInCircle(p2))
	{
		minRad2 = circles[0].GetRadius();
		index = 0;
	}

	// construct a circle from p1 and p3 and check if p0 and p2 are inside it
	circles[1] = CreateCircle(p1, p3);
	if (circles[1].GetRadius() < minRad2 &&
		circles[1].IsInCircle(p0) && circles[1].IsInCircle(p2))
	{
		minRad2 = circles[1].GetRadius();
		index = 1;
	}

	// construct a circle from p2 and p3 and check if p0 and p1 are inside it
	circles[2] = CreateCircle(p2, p3);
	if (circles[2].GetRadius() < minRad2 &&
		circles[2].IsInCircle(p0) && circles[2].IsInCircle(p1))
	{
		minRad2 = circles[2].GetRadius();
		index = 2;
	}

	// construct a circle from p0, p1 and p3 and check if p2 is inside it
	circles[3] = CreateCircle(p0, p1, p3);
	if (circles[3].GetRadius() < minRad2 && circles[3].IsInCircle(p2))
	{
		minRad2 = circles[3].GetRadius();
		index = 3;
	}

	// construct a circle from p0, p2 and p3 and check if p1 is inside it
	circles[4] = CreateCircle(p0, p2, p3);
	if (circles[4].GetRadius() < minRad2 && circles[4].IsInCircle(p1))
	{
		minRad2 = circles[4].GetRadius();
		index = 4;
	}

	// construct a circle from p1, p2 and p3 and check if p0 is inside it
	circles[5] = CreateCircle(p1, p2, p3);
	if (circles[5].GetRadius() < minRad2 && circles[5].IsInCircle(p0))
	{
		minRad2 = circles[5].GetRadius();
		index = 5;
	}

	// get the minimum circle
	Circle minc = circles[index];

	// update set of support
	switch (index)
	{
	case 0:
		// two points, p replaces second point
		boundaryPoints.resize(2);
		boundaryPoints[1] = point;
		break;
	case 1:
		// two points, p replaces first point
		boundaryPoints.resize(2);
		boundaryPoints[0] = point;
		break;
	case 2:
	{
		glm::vec3 temp = boundaryPoints[2];
		boundaryPoints.resize(2);
		boundaryPoints[0] = temp;
		boundaryPoints[1] = point;
		break;
	}
	case 3:
		// three points, p replaces third point
		boundaryPoints[2] = point;
		break;
	case 4:
		// three points, p replaces second point
		boundaryPoints[1] = point;
		break;
	case 5:
		// three points, p replaces first point
		boundaryPoints[0] = point;
		break;
	}

	return minc;
}

Circle* ObjectManager::WelzlMBC(int n, std::vector<glm::vec3> boundaryPoints)
{
	Circle* result;

	//if (boundaryPoints.size() == 3)
	//	result = CreateCircle(boundaryPoints[0], boundaryPoints[1], boundaryPoints[2]);
	//else if (n == 1 && boundaryPoints.size() == 0)
	//{
	//	result = new Circle();
	//	result->SetCenter(m_points[0].GetPosition());
	//	result->SetRadius(0);
	//}
	//else if (n == 0 && boundaryPoints.size() == 2)
	//	result = CreateCircle(boundaryPoints[0], boundaryPoints[1]);
	//else if (n == 1 && boundaryPoints.size() == 1)
	//	result = CreateCircle(boundaryPoints[0], m_points[0].GetPosition());
	//else
	//{
	//	result = WelzlMBC(n - 1, boundaryPoints);
	//	if (!result->IsInCircle(m_points[n - 1].GetPosition()))
	//	{
	//		delete result;
	//		boundaryPoints.push_back(m_points[n - 1].GetPosition());
	//		result = WelzlMBC(n - 1, boundaryPoints);
	//	}
	//}

	return result;
}

Circle ObjectManager::CreateCircle(glm::vec3 point1, glm::vec3 point2)
{
	Circle result;

	double p1x = point1.x;
	double p1y = point1.y;
	double p2x = point2.x;
	double p2y = point2.y;

	double cx = 0.5*(p1x + p2x);
	double cy = 0.5*(p1y + p2y);
	
	result.SetCenter(glm::vec3(cx, cy, 0));
	result.SetRadius(sqrt((p1x - cx)*(p1x - cx) + (p1y - cy)*(p1y - cy)));
	return result;
	//double p1x = point1.x;
	//double p1y = point1.y;
	//double p2x = point2.x;
	//double p2y = point2.y;
	//					 
	//double cx = 0.5*(p1x + p2x);
	//double cy = 0.5*(p1y + p2y);

	//double xDist = p1x - p2x;
	//double yDist = p1y - p2y;
	//double dist = sqrt(xDist*xDist + yDist*yDist);

	//Circle circle;
	//circle.SetCenter(glm::vec3(cx, cy, 0));
	//circle.SetRadius(dist * 0.5);

	//return circle;
}

Circle ObjectManager::CreateCircle(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3)
{
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

	/*Circle circle;

	double xDistance, yDistance, side_a, side_b, side_c, s;
	double term1, term2, term3, denom, cx, cy;

	double p1x = point1.x;
	double p1y = point1.y;
	double p2x = point2.x;
	double p2y = point2.y;
	double p3x = point3.x;
	double p3y = point3.y;

	xDistance = p1x - p2x;
	yDistance = p1y - p2y;
	side_a = sqrt(xDistance*xDistance + yDistance * yDistance);

	xDistance = p2x - p3x;
	yDistance = p2y - p3y;
	side_b = sqrt(xDistance*xDistance + yDistance * yDistance);

	xDistance = p3x - p1x;
	yDistance = p3y - p1y;
	side_c = sqrt(xDistance*xDistance + yDistance * yDistance);

	s = (side_a + side_b + side_c)*0.5;
	denom = sqrt(s*(s - side_a)*(s - side_b)*(s - side_c));
	circle.SetRadius(side_a * side_b * side_c * 0.25 / denom);

	term1 = p1x*p1x + p1y + p1y;
	term2 = p2x*p2x + p2y + p2y;
	term3 = p3x*p3x + p3y*p3y;

	denom = 2.0 * (p1x * (p2y - p3y) +
		p2x * (p3y - p1y) +
		p3x * (p1y - p2y));

	cx = (term1 * (p2y - p3y) + term2 * (p3y - p1y) +
		term3 * (p1y - p2y)) / denom;
	cy = (term1 * (p3x - p2x) + term2 * (p1x - p3x) +
		term3 * (p2x - p1x)) / denom;
	
	circle.SetCenter(glm::vec3(cx, cy, 0));
	
	return circle;*/
}

const double ObjectManager::EPSILON = 0.000000000001;