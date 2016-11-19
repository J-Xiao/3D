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
	glTranslatef(-5, 4, -13);
	glRotatef(0, 1.0, 1.0, 1.0);
	


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