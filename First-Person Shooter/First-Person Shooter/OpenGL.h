#include "stdafx.h"

#pragma once

class OpenGL
{
public:
	OpenGL();
	virtual ~OpenGL();

	HDC hDC;
	HGLRC hRC;

	BOOL SetupPixelFormat(HDC hDC);
	void init(int width, int height);
	void Render();
	void CleanUp();
};

