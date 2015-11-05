#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <string>
#include <vector>

GLuint LoadDDS(std::string filepath);
GLuint LoadBMP(std::string filepath);

std::vector<glm::vec2> FloatVecToGLMVec2(std::vector<float> vec);
std::vector<glm::vec3> FloatVecToGLMVec3(std::vector<float> vec);
void computeTangentBasis(
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals,
	std::vector<glm::vec3> & tangents,
	std::vector<glm::vec3> & bitangents);

void indexVBO_TBN(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals,
	std::vector<glm::vec3> & in_tangents,
	std::vector<glm::vec3> & in_bitangents,

	std::vector<unsigned short> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals,
	std::vector<glm::vec3> & out_tangents,
	std::vector<glm::vec3> & out_bitangents);

std::string ReadFileToString(const char* filePath);
GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile);
GLuint CreateProgram(const std::vector<GLuint> &shaderList);

std::vector<glm::vec3> CreateOctahedronWithRadius(glm::vec3 center, float radius);
std::vector<glm::vec3> CreateOctahedronWithSide(glm::vec3 center, float side);

//Splits an equilater triangle into 4 equilateral parts
std::vector<glm::vec3> DivideTriangle(glm::vec3 octahedronVertices[3], int remainingSubdivisons);
std::vector<glm::vec3> OctahedronToSphere(std::vector<glm::vec3> octahedronFaces, glm::vec3 sphereCenter, float sphereRadius, int numSubdivisions);