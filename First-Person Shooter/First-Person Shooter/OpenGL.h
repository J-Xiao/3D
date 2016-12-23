#pragma once

class OpenGL
{
public:
	OpenGL();
	virtual ~OpenGL();

	HDC hDC;
	HGLRC hRC;
	BOOL SetupPixelFormat(HDC hDC);
	void Init(float width, float height);
	void Render();
	void CleanUp();
};

void drawPoint();
void drawTriangle();
void drawSquare();
void drawCube();
void drawCircle();
void drawPillar();