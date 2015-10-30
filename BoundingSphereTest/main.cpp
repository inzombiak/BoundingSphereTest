#include <iostream>
#define FREEGLUT_STATIC
#define GLEW_STATIC
#include <gl\glew.h>
#include <gl\freeglut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "ObjectManager.h"

int m_x = 0, m_y = 0, m_mouseClickX = 0.0f, m_mouseClickY = 0.0f;
float m_yRotate = 0.f, m_xRotate = 3.14f, deltaTime = 1;
bool mouseDown = false;

// position
glm::vec3 position = glm::vec3(0, 0, 10), right, up, direction;
// Initial Field of View

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;

ObjectManager m_om;

/*
std::string ReadFileToString(const char* filePath)
{
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

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
	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
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
	GLuint program = glCreateProgram();

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
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

void InitializeProgram()
{
	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, ReadFileToString("VertexShader.glsl")));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, ReadFileToString("FragmentShader.glsl")));
	theProgram = CreateProgram(shaderList);

	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
	shaderList.clear();

	//shaderList.push_back(CreateShader(GL_VERTEX_SHADER, ReadFileToString("NormalVertexShader.glsl")));
	//shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, ReadFileToString("NormalFragmentShader.glsl")));
	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, ReadFileToString("TextureVertexShader.glsl")));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, ReadFileToString("TextureFragmentShader.glsl")));
	textureProgram = CreateProgram(shaderList);

	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}*/

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
	m_om.Init();
	m_om.GenerateRandomPoints(4000, glm::vec2(-0.7, 0.7), glm::vec2(-0.7, 0.7), glm::vec2(-0.7, 0.7));
}

void reInit()
{
	m_om.Clear();
	m_om.GenerateRandomPoints(4000, glm::vec2(-0.7, 0.7), glm::vec2(-0.7, 0.7), glm::vec2(-0.7, 0.7));
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		return;
	case 'd':
		position += right * speed;
		glutPostRedisplay();
		return;
	case 'a':
		position -= right * speed;
		glutPostRedisplay();
		return;
	case 's':
		position -= direction * speed;
		glutPostRedisplay();
		return;
	case 'w':
		position += direction  * speed;
		glutPostRedisplay();
	case 'c':
		m_om.CalculateBoundingCircle();
		glutPostRedisplay();
		return;
	case 'r':
		reInit();
		glutPostRedisplay();
		return;
	}

}

void display()
{

	direction = glm::vec3(
		cos(m_yRotate) * sin(m_xRotate),
		sin(m_yRotate),
		cos(m_yRotate) * cos(m_xRotate)
		);

	right = glm::vec3(
		sin(m_xRotate - 3.14f / 2.0f),
		0,
		cos(m_xRotate - 3.14f / 2.0f)
		);
	up = glm::cross(right, direction);
	
	m_om.Draw(position, direction, up);
}

void mouse(int button, int state, int x, int y)
{

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			m_mouseClickX = x;
			m_mouseClickY = y;
			mouseDown = true;
		}
		else if (state == GLUT_UP)
		{
			mouseDown = false;

		}
		break;
	}
}
void drag(int x, int y)
{
	m_xRotate += mouseSpeed * float(glutGet(GLUT_WINDOW_WIDTH) / 2 - x);
	m_yRotate += mouseSpeed * float(glutGet(GLUT_WINDOW_HEIGHT) / 2 - y);

	display();
	if (x != glutGet(GLUT_WINDOW_WIDTH) / 2 || y != glutGet(GLUT_WINDOW_HEIGHT) / 2)
		glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("3D example");
	glewInit();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(drag);
	init();
	glutMainLoop();
}