#pragma once

class OpenGL
{
public:
	OpenGL();
	virtual ~OpenGL();

	HDC hDC;
	HGLRC hRC;
	class BasicShapes *m_basicShapes;
	BOOL SetupPixelFormat(HDC hDC);
	void Init(float width, float height);
	void Render();
	void CleanUp();

private:
	class Camera* m_camera;
};

