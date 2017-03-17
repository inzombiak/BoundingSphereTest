#include "RenderComponent.h"


void RenderComponent::Draw(glm::mat4 mvp)
{
	//Set program
	glUseProgram(m_program);

	//Enable attributes for use
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

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBufferObject);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	//Set draw mode
	if (m_drawPrimitive == GL_POINTS)
		glPolygonMode(GL_FRONT, GL_POINT);
	else
		glPolygonMode(GL_FRONT, GL_LINE);

	//Pass in MVP
	GLuint MVPMatID = glGetUniformLocation(m_program, "MVP");
	glUniformMatrix4fv(MVPMatID, 1, GL_FALSE, &mvp[0][0]);

	//Draw
	glDrawArrays(m_drawPrimitive, 0, m_numVertices);
}

void RenderComponent::SetProgram(GLuint program)
{
	m_program = program;
}

void RenderComponent::SetVertices(std::vector<glm::vec3> vertices)
{
	//If not buffer exists, create one
	if (m_vertexBufferObject == 0)
		glGenBuffers(1, &m_vertexBufferObject);

	//Set data
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	m_numVertices = vertices.size();
}

void RenderComponent::SetColor(std::vector<glm::vec3> color)
{
	if (color.size() < m_numVertices)
	{
		color.resize(m_numVertices, glm::vec3(0.f, 0.f, 0.f));
	}


	//Same as above
	if (m_colorBufferObject == 0)
		glGenBuffers(1, &m_colorBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBufferObject);
	glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(glm::vec3), &color[0], GL_STATIC_DRAW);
}

void RenderComponent::SetDrawPrimitive(GLuint type)
{
	m_drawPrimitive = type;
}