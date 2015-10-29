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
	//test = new Circle(m_points[1].GetPosition(), 2, m_renderManager.CreateRenderComponent());

	//position.x = 0.5f;
	//position.y = 0.0f;
	//position.z = 0.0f;
	//Point newPoint1(position, color, m_renderManager.CreateRenderComponent());
	//position.x = 0.0f;
	//position.y = 0.5f;
	//Point newPoint2(position, color, m_renderManager.CreateRenderComponent());

	//position.x = -0.5f;
	//position.y = 0.0f;
	//Point newPoint3(position, color, m_renderManager.CreateRenderComponent());
}
void ObjectManager::CalculateBoundingCircle()
{
	std::vector<glm::vec3> boundaryPoints;

	Circle MBC = WelzlMBC(m_points.size(), boundaryPoints);
	MBC.SetRenderComponent(m_renderManager.CreateRenderComponent());

	printf("Radius is %f, ", MBC.GetRadius());
	printf("Center is %f \n", MBC.GetCenter().x);

}
void ObjectManager::Draw(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{
	m_renderManager.Draw(position, direction, up);
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

Circle ObjectManager::WelzlMBC(int n, std::vector<glm::vec3> boundaryPoints)
{
	Circle result;
	float xDist, yDist, radius2, distance2;

	if (boundaryPoints.size() == 3)
		result = CreateCircle(boundaryPoints[0], boundaryPoints[1], boundaryPoints[2]);
	else if (n == 1 && boundaryPoints.size() == 0)
	{
		result.SetCenter(m_points[0].GetPosition());
		result.SetRadius(0);
	}
	else if (n == 0 && boundaryPoints.size() == 2)
		result = CreateCircle(boundaryPoints[0], boundaryPoints[1]);
	else if (n == 1 && boundaryPoints.size() == 1)
		result = CreateCircle(boundaryPoints[0], m_points[0].GetPosition());
	else
	{
		result = WelzlMBC(n - 1, boundaryPoints);
		if (!result.IsInCircle(m_points[n - 1].GetPosition()))
		{
			boundaryPoints.push_back(m_points[n - 1].GetPosition());
			result = WelzlMBC(n - 1, boundaryPoints);
		}
	}

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