#include <iostream>
#define GLEW_STATIC
#include "glew.h"
#include "gl\freeglut.h""
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "ObjectManager.h"

//Number of points to be genereated
int m_numberOfPoints = 6;

//Camera rotation along x and y axes
float m_yRotate = 0.f, m_xRotate = 3.14f;
float deltaTime = 1;

//Should draw 2D or 3D
bool m_draw3D = false;

//X, Y and Z bounds for the random points
glm::vec2 m_xBounds(-0.7, 0.7);
glm::vec2 m_yBounds(-0.7, 0.7);
glm::vec2 m_zBounds(-0.7, 0.7);

//Used for camera speed and positioning
glm::vec3 position = glm::vec3(0, 0, 5), right, up, direction;
float speed = 1.0f;
float mouseSpeed = 0.005f;

//Object manager
ObjectManager m_om;

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
	//Initalize the object manager
	m_om.Init();

	//Depending on draw mode generate either 2D or 3D points
	if (!m_draw3D)
		m_om.GenerateRandomPoints2D(m_numberOfPoints, m_xBounds, m_yBounds);
	else
		m_om.GenerateRandomPoints3D(m_numberOfPoints, m_xBounds, m_yBounds, m_zBounds);
}

void reInit()
{
	//Clear existing data
	m_om.Clear();

	//Reset camera
	m_yRotate = 0.f;
	m_xRotate = 3.14f;
	deltaTime = 1;
	position = glm::vec3(0, 0, 5);

	//Regenerate points
	if (!m_draw3D)
		m_om.GenerateRandomPoints2D(m_numberOfPoints, m_xBounds, m_yBounds);
	else							
		m_om.GenerateRandomPoints3D(m_numberOfPoints, m_xBounds, m_yBounds, m_zBounds);

	glutPostRedisplay();
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
		return;
	case 'c':
		m_om.CalculateBoundingShape();
		glutPostRedisplay();
		return;
	case 'r':
		reInit();
		return;
	case 'f' :
		m_draw3D = !m_draw3D;
		reInit();
		return;
	}

}

void display()
{
	//Calculate camera settings
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
	
	//Draw screen
	m_om.Draw(position, direction, up);
}

bool m_mouseDown = false;
int m_oldMouseX, m_oldMouseY;

void drag(int x, int y)
{
	//Only drag if clicked
	if (!m_mouseDown)
		return;

	//Rotate camera based on mouse movement
	m_xRotate += mouseSpeed * float(m_oldMouseX - x);
	m_yRotate += mouseSpeed * float(m_oldMouseY - y);

	//Redisplay
	display();

	m_oldMouseX = x;
	m_oldMouseY = y;
}

void mouseButtonPressed(int button, int state, int x, int y)
{
	if (button != GLUT_LEFT_BUTTON)
		return;

	if (state == GLUT_UP)
	{
		m_mouseDown = false;
	}
	else
	{
		m_mouseDown = true;
	}
		
	m_oldMouseX = x;
	m_oldMouseY = y;

}

int main(int argc, char **argv)
{

	std::cout << "Enter number of points, please use integer numbers: ";
	std::cin >> m_numberOfPoints;

	//OpenGL and GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("3D example");
	glewInit();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseButtonPressed);
	glutMotionFunc(drag);
	init();
	glutMainLoop();
}