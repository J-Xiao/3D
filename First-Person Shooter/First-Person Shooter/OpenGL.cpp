#include "stdafx.h"
#include "OpenGL.h"
#include "BasicShapes.h"
#include "Camera.h"

OpenGL::OpenGL()
{
	hRC = NULL;
	hDC = NULL;
}

OpenGL::~OpenGL()
{
	CleanUp();
}

BOOL OpenGL::SetupPixelFormat(HDC hdc) {
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

void OpenGL::Init(float width, float height)
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

	m_basicShapes = new BasicShapes();
	m_camera = new Camera();
}

void OpenGL::Render() {
	glClearColor(0.2f, 0.5f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	m_camera->DisplayScene();
	m_camera->DrawGround();

	glColor3f(0, 0, 0);

	glPushMatrix();
	glTranslatef(5, 4, -13);
	glRotatef(0, 1.0, 1.0, 1.0);
		BasicShapes::DrawTriangle();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5, 0, -13);
	glRotatef(20, 1.0, 1.0, 1.0);
		BasicShapes::DrawSquare();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, -13);
	glRotatef(20, 1.0, 1.0, 1.0);
		BasicShapes::DrawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5, 0, -13);
	glRotatef(0, 1.0, 1.0, 1.0);
		BasicShapes::DrawCircle();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5, -4, -13);
	glRotatef(0, 1.0, 1.0, 1.0);
		BasicShapes::DrawPillar();
	glPopMatrix();

	//glPushMatrix();
	//glTranslatef(0, -8, -13);
	//glRotatef(90, 1.0, 1.0, 0.0);
	//glPopMatrix();

	m_basicShapes->Airplane(0, 8, -50);

	glFlush();

	SwapBuffers(hDC);
}

void OpenGL::CleanUp() {
	wglMakeCurrent(hDC, NULL);
	wglDeleteContext(hRC);
}

