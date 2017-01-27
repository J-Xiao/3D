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
	delete m_basicShapes;
	delete m_camera;
	CleanUp();
}

BOOL OpenGL::SetupPixelFormat(HDC hdc) {
	this->hDC = hdc;

	int nPixelFormat;
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
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
		0, 0, 0
	};

	if (!(nPixelFormat = ChoosePixelFormat(hDC, &pfd))) {
		MessageBox(NULL, L"Not found", L"Error", MB_OK | MB_ICONEXCLAMATION);
	}

	SetPixelFormat(hDC, nPixelFormat, &pfd);
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	m_basicShapes = new BasicShapes();
	m_camera = new Camera();

	return TRUE;
}

void OpenGL::Init(float width, float height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		54.0f,
		(GLfloat) width / (GLfloat) height,
		0.1f,
		3000.0f
	);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void OpenGL::Render() {
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	m_camera->DisplayScene();
	m_camera->CreateSkyBox(3,6,3,6);
	m_camera->DrawTerrain();
	//m_camera->DrawGround();
	srand(100);
	for (int i = 0; i<300; i++)
	{
		float x = RAND_COORD((MAP_W - 1)*MAP_SCALE);
		float z = RAND_COORD((MAP_W - 1)*MAP_SCALE);
		float size = 4.0f + rand() % 4;
		float h = -size / 10;
		int   cactus = rand() % 4 + 11;
		m_camera->ShowTree0(x, z, size, h, cactus);
	}

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

	m_basicShapes->Airplane(MAP + 10, 80, -MAP);

	glFlush();

	SwapBuffers(hDC);
}

void OpenGL::CleanUp() {
	wglMakeCurrent(hDC, NULL);
	wglDeleteContext(hRC);
}
