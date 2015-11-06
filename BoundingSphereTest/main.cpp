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

const int NUMBER_OF_POINTS = 12;

int m_x = 0, m_y = 0;
float m_yRotate = 0.f, m_xRotate = 3.14f, deltaTime = 1;
bool m_draw3D = false;
// position
glm::vec3 position = glm::vec3(0, 0, 10), right, up, direction;
float speed = 1.0f;
float mouseSpeed = 0.005f;

ObjectManager m_om;

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
	m_om.Init();

	if (!m_draw3D)
		m_om.GenerateRandomPoints2D(NUMBER_OF_POINTS, glm::vec2(-0.7, 0.7), glm::vec2(-0.7, 0.7));
	else
		m_om.GenerateRandomPoints3D(NUMBER_OF_POINTS, glm::vec2(-0.7, 0.7), glm::vec2(-0.7, 0.7), glm::vec2(-0.7, 0.7));
}

void reInit()
{
	m_om.Clear();

	if (!m_draw3D)
		m_om.GenerateRandomPoints2D(NUMBER_OF_POINTS, glm::vec2(-0.7, 0.7), glm::vec2(-0.7, 0.7));
	else
		m_om.GenerateRandomPoints3D(NUMBER_OF_POINTS, glm::vec2(-0.7, 0.7), glm::vec2(-0.7, 0.7), glm::vec2(-0.7, 0.7));

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