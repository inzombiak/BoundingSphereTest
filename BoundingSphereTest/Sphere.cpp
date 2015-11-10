#include "Sphere.h"
#include "HelperFunctions.h"
#include <vector>

void Sphere::SetCenter(glm::vec3 position)
{
	m_center = position;
	if (m_renderComponent != 0)
		UpdateRenderComponent();

}

void Sphere::SetRadius(float radius)
{
	m_radius = radius;
	if (m_renderComponent)
		UpdateRenderComponent();
}

void Sphere::UpdateRenderComponent()
{
	std::vector<glm::vec3> octahedron;
	//Create an octahedron using the spheres center and radius
	octahedron = CreateOctahedronWithRadius(m_center, m_radius);
	//Convert octahedron to sphere
	std::vector<glm::vec3> vertexBuffer = OctahedronToSphere(octahedron, m_center, m_radius, NUMBER_OF_SPLITS);
	//If we have a render component, set its vertex buffer
	if (m_renderComponent)
		m_renderComponent->SetVertices(vertexBuffer);
}