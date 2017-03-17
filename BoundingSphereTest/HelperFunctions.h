#pragma once

#include "glew.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <string>
#include <vector>

//Shader compliation helped funcitons
//Reads a file to a std::string
std::string ReadFileToString(const char* filePath);
//Creates a GLenum type shader from a string
GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile);
//Compiles shaders into a program
GLuint CreateProgram(const std::vector<GLuint> &shaderList);

//Sphere generation functions
//Creates an octohedron using a center and radius
std::vector<glm::vec3> CreateOctahedronWithRadius(glm::vec3 center, float radius);
//Creates an octohedron using a center and side length
std::vector<glm::vec3> CreateOctahedronWithSide(glm::vec3 center, float side);

//Splits an equilater triangle into 4 equilateral parts. First varable is an array of the 3 vertices of the triangle, 2nd variable is the number of remaining subdivisions
std::vector<glm::vec3> DivideTriangle(glm::vec3 octahedronVertices[3], int remainingSubdivisons);
//Converts an octohedron with the given faces(provided as individual vertices) to a sphere with the given center and radius. Number of subdivisions is number of times each face should be divided
std::vector<glm::vec3> OctahedronToSphere(std::vector<glm::vec3> octahedronFaces, glm::vec3 sphereCenter, float sphereRadius, int numSubdivisions);