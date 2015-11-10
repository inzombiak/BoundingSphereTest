#include "HelperFunctions.h"
#include <ddraw.h>
#include <map>
#include <fstream>
#include <iostream>
std::string ReadFileToString(const char* filePath)
{
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);
	//Make sure file is open
	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	//Read contents to a string
	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}
GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile)
{
	//Create shader of type
	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = strShaderFile.c_str();
	//Set the sourse
	glShaderSource(shader, 1, &strFileData, NULL);
	//Compile the shader
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	//If there was an error, get it and display the message
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}
GLuint CreateProgram(const std::vector<GLuint> &shaderList)
{
	//Create a new program
	GLuint program = glCreateProgram();
	//Attach shaders to it
	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);
	//Link program
	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	//If there was an error, get it and display the message
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}

std::vector<glm::vec3> CreateOctahedronWithRadius(glm::vec3 center, float radius)
{
	//Calculate side length and pass to other octahedron function

	float side;
	side = radius * sqrt(2);

	return CreateOctahedronWithSide(center, side);
}
std::vector<glm::vec3> CreateOctahedronWithSide(glm::vec3 center, float side)
{
	std::vector<glm::vec3> result;

	//Vertices of the Octahedron
	glm::vec3 top, bottom, cornerFrontLeft, corners[4];

	//Calculate positions of the vertices
	top = center;
	top.y += side / sqrt(2);

	bottom = center;
	bottom.y -= side/ sqrt(2);

	cornerFrontLeft = center;
	cornerFrontLeft.x -= side / 2;
	cornerFrontLeft.z -= side / 2;

	corners[0] = center;
	corners[0].x -= side / 2;
	corners[0].z -= side / 2;

	corners[1] = corners[0];
	corners[1].x += side;

	corners[2] = corners[1];
	corners[2].z += side;

	corners[3] = corners[2];
	corners[3].x -= side;

	//Create top 4 faces
	for (unsigned int i = 0; i < 4; ++i)
	{
		result.push_back(top);
		result.push_back(corners[i]);
		result.push_back(corners[(i + 1) % 4]);
	}
	//Create bottom four faces
	for (unsigned int i = 0; i < 4; ++i)
	{
		result.push_back(bottom);
		result.push_back(corners[i]);
		result.push_back(corners[(i + 1) % 4]);
	}

	return result;
}

std::vector<glm::vec3> OctahedronToSphere(std::vector<glm::vec3> octahedronVertices, glm::vec3 sphereCenter, float sphereRadius, int numSubdivisions)
{
	//Create vector to hold final vertices
	std::vector<glm::vec3> result;
	//Reerve amount of space that will be needed, 8 faces with 4^n triangles each with 3 vertices
	result.reserve(unsigned int(pow(4, numSubdivisions) * 3 * 8));

	unsigned int numVertices = octahedronVertices.size();

	//Make sure we have a octahedron, or at least the right number of vertices
	if (numVertices != 24)
		return result;

	std::vector<glm::vec3> splitResults;
	glm::vec3 faceVertices[3];

	//Perfrom split on each face
	for (unsigned int i = 0; i < numVertices; i += 3)
	{
		faceVertices[0] = octahedronVertices[i];
		faceVertices[1] = octahedronVertices[i + 2];
		faceVertices[2] = octahedronVertices[i + 1];
		//Call recrusive split function
		splitResults = DivideTriangle(faceVertices, numSubdivisions);
		//Append results to existing vector
		result.insert(result.begin(), splitResults.begin(), splitResults.end());
	}

	/*
	We turn the split octahedron into a sphere by "wrapping" the vertices around the center.
	This means we repeatedly loop over the vertices and move all of them to be radius distance from the center.	
	*/

	//Number of vertices, should be equal to reserved space
	int finalVertexCount = result.size();
	//Distance from center of the sphere
	float distanceFromCenter;
	//Vector from current point to the center of the sphere
	glm::vec3 pointToCenterVector;
	//New point
	glm::vec3 newPoint;

	//Perfrom looping operation 8 time. The more time we loop the more precise it will be. I found 8 to be enough
	for (int j = 0; j < 8; ++j)
	{
		//Loop over all vertices
		for (unsigned int i = 0; i < finalVertexCount; ++i)
		{
			//Ger the distance from the center for current point
			distanceFromCenter = glm::distance(sphereCenter, result[i]);
			
			//Construct a vector from point to center and "normalize" it
			pointToCenterVector = (result[i] - sphereCenter) * sphereRadius * (1 / distanceFromCenter);

			//Weird/cool result 
			//newPoint = result[i] + pointToCenterVector * (sphereRadius);

			//Move the point closer to the center
			newPoint = sphereCenter + pointToCenterVector;
			//Replace old point with new one
			result[i] = newPoint;
		}
	}
		
	return result;
}
std::vector<glm::vec3> DivideTriangle(glm::vec3 originalVertices[3],  int remainingSubdivisons)
{
	std::vector<glm::vec3> result;
	glm::vec3 midPoints[3];

	//Get mid points of the sides
	midPoints[0] = glm::vec3((originalVertices[0].x + originalVertices[1].x) / 2, (originalVertices[0].y + originalVertices[1].y) / 2, (originalVertices[0].z + originalVertices[1].z) / 2);
	midPoints[1] = glm::vec3((originalVertices[1].x + originalVertices[2].x) / 2, (originalVertices[1].y + originalVertices[2].y) / 2, (originalVertices[1].z + originalVertices[2].z) / 2);
	midPoints[2] = glm::vec3((originalVertices[2].x + originalVertices[0].x) / 2, (originalVertices[2].y + originalVertices[0].y) / 2, (originalVertices[2].z + originalVertices[0].z) / 2);

	//Construct "outer" 3 split triangles. These are the ones that use one of the original triangles vertices
	for (int i = 0; i < 3; ++i)
	{
		result.push_back(midPoints[i]);
		result.push_back(originalVertices[i]);
		result.push_back(midPoints[(i + 2) % 3]);
	}
	//Append the central triangle, made up of only the mdipoints
	result.insert(result.begin(), midPoints, midPoints + 3);

	//If we have no remaining subdivisions, terminate recursion
	if (remainingSubdivisons == 0)
		return result;

	//Otherwise recursively split each of the newly formed triangles
	//Results from recursive split
	std::vector<glm::vec3> splitResults;
	//Holds the vertices for each new face
	glm::vec3 faceVertices[3];
	//New final result
	std::vector<glm::vec3> newResult;

	//Loop over all the faces
	for (unsigned int i = 0; i < result.size(); i+=3)
	{
		//Set the face vertices
		faceVertices[0] = result[i];
		faceVertices[1] = result[i + 2];
		faceVertices[2] = result[i + 1];

		//Perfrom split
		splitResults = DivideTriangle(faceVertices, remainingSubdivisons - 1);
		//Append results
		newResult.insert(newResult.begin(), splitResults.begin(), splitResults.end());
	}

	return newResult;
}
