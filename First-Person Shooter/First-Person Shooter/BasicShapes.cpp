#include "stdafx.h"
#include "BasicShapes.h"


BasicShapes::BasicShapes()
{
}


BasicShapes::~BasicShapes()
{
}

void BasicShapes::DrawPoint() {
	glBegin(GL_POINTS);
	glVertex3f(0.0, 1.0, -1.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glEnd();
}

void BasicShapes::DrawLine()
{
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, 1.0, -1.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glEnd();
}

void BasicShapes::DrawTriangle()
{
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 1.0, -1.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glEnd();
}

void BasicShapes::DrawSquare()
{
	glBegin(GL_POLYGON);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glEnd();
}

void BasicShapes::DrawCube()
{
	glBegin(GL_QUAD_STRIP);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 1.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(0.0f, 1.0f, -1.0f);
	glEnd();
}

void BasicShapes::DrawCircle()
{
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 0.0f);
	for (int i = 0; i < 390; i += 30)
	{
		float p = (float)(i * 3.14 / 180);
		glVertex3f((float)sin(p), (float)cos(p), 0.0f);
	}
	glEnd();
}

void BasicShapes::DrawPillar()
{
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < 390; i += 30)
	{
		float p = (float)(i * 3.14 / 180);
		glVertex3f((float)(sin(p) / 2), (float)(cos(p) / 2), 1.0f);
		glVertex3f((float)(sin(p) / 2), (float)(cos(p) / 2), 0.0f);
	}
	glEnd();
}
