#include "RenderComponent.h"


void RenderComponent::Draw(glm::mat4 mvp)
{
	glUseProgram(m_program);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	if (m_drawPrimitive == GL_POINTS)
		glPolygonMode(GL_FRONT, GL_POINT);
	else
		glPolygonMode(GL_FRONT, GL_LINE);

	glDrawArrays(m_drawPrimitive, 0, m_numVertices);
}

void RenderComponent::SetProgram(GLuint program)
{
	m_program = program;
}

void RenderComponent::SetVertices(std::vector<glm::vec3> vertices)
{
	if (m_vertexBufferObject == 0)
		glGenBuffers(1, &m_vertexBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	m_numVertices = vertices.size();
}

void RenderComponent::SetColor(std::vector<glm::vec3> color)
{
	if (m_colorBufferObject == 0)
		glGenBuffers(1, &m_colorBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBufferObject);
	glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(glm::vec3), &color[0], GL_STATIC_DRAW);
}

void RenderComponent::SetDrawPrimitive(GLuint type)
{
	m_drawPrimitive = type;
}