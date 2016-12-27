#pragma once

class BasicShapes
{
public:
	BasicShapes();
	virtual ~BasicShapes();
	static void DrawPoint();
	static void DrawLine();
	static void DrawTriangle();
	static void DrawSquare();
	static void DrawCube();
	static void DrawCircle();
	static void DrawPillar();
	void Airplane(float x, float y, float z);
	void Box(float x, float y, float z);

private:
	GLuint m_planeMap[2];
	GLfloat angle;
	GLUquadricObj* m_quadricObj;
	bool LoadT8Map(char *fileName, GLuint &texture);
};

