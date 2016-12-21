#include "stdafx.h"
#include "OpenGL.h"

OpenGL::OpenGL()
{
	hRC = NULL;
	hDC = NULL;
}

OpenGL::~OpenGL()
{
	cleanUp();
}

BOOL OpenGL::setupPixelFormat(HDC hdc) {
	this->hDC = hdc;

	int nPixelFormat;
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW,
		PFD_SUPPORT_OPENGL,
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		16,
		0,0,0,0,0,0,
		0,
		0,
		0,
		0,0,0,0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0
	};

	if (!(nPixelFormat = ChoosePixelFormat(hDC, &pfd))) {
		MessageBox(NULL, L"Not found", L"Error", MB_OK | MB_ICONEXCLAMATION);
	}

	SetPixelFormat(hDC, nPixelFormat, &pfd);
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);
	return TRUE;
}

void OpenGL::init(float width, float height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		45.0f,
		width / height,
		1.0f,
		1000.0f
	);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void OpenGL::render() {
	glClearColor(0.0f, 0.0f, 0.8f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glColor3f(0, 0, 0);

	glPushMatrix();
	glTranslatef(5, 4, -13);
	glRotatef(0, 1.0, 1.0, 1.0);
	drawTriangle();

	glPopMatrix();
	glPushMatrix();
	glTranslatef(-5, 0, -13);
	glRotatef(20, 1.0, 1.0, 1.0);
	drawSquare();

	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, -13);
	glRotatef(20, 1.0, 1.0, 1.0);
	drawCube();

	glPopMatrix();
	glPushMatrix();
	glTranslatef(5, 0, -13);
	glRotatef(0, 1.0, 1.0, 1.0);
	drawCircle();

	glPopMatrix();
	glPushMatrix();
	glTranslatef(-5, -4, -13);
	glRotatef(0, 1.0, 1.0, 1.0);
	drawPillar();

	glFlush();

	SwapBuffers(hDC);
}

void OpenGL::cleanUp() {
	wglMakeCurrent(hDC, NULL);
	wglDeleteContext(hRC);
}

void drawPoint() {
	glBegin(GL_POINTS);
	glVertex3f(0.0, 1.0, -1.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glEnd();
}

void drawLine()
{
	glBegin(GL_LINE_LOOP);
		glVertex3f(0.0, 1.0, -1.0);
		glVertex3f(-1.0, -1.0, 0.0);
		glVertex3f(1.0, -1.0, 0.0);
	glEnd();
}

void drawTriangle()
{
	glBegin(GL_POLYGON);
		glVertex3f(0.0, 1.0, -1.0);
		glVertex3f(-1.0, -1.0, 0.0);
		glVertex3f(1.0, -1.0, 0.0);
	glEnd();
}

void drawSquare()
{
	glBegin(GL_POLYGON);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, -1.0f);
		glVertex3f(0.0f, 0.0f, -1.0f);
	glEnd();
}

void drawCube()
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

void drawCircle()
{
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0, 0, 0.0f);
		for (int i = 0; i < 390; i += 30)
		{
			float p = (float) (i * 3.14 / 180);
			glVertex3f((float)sin(p), (float)cos(p), 0.0f);
		}
	glEnd();
}

void drawPillar()
{
	glBegin(GL_QUAD_STRIP);
		for(int i = 0; i < 390; i += 30)
		{
			float p = (float)(i * 3.14 / 180);
			glVertex3f((float)(sin(p)/2), (float)(cos(p)/2), 1.0f);
			glVertex3f((float)(sin(p)/2), (float)(cos(p)/2), 0.0f);
		}
	glEnd();
}