#include "stdafx.h"
#include "BasicShapes.h"

GLfloat angle = 0;

BasicShapes::BasicShapes()
{
 	m_quadricObj = gluNewQuadric();
	LoadT8Map("aa.bmp", m_planeMap[0]);
	LoadT8Map("bb.bmp", m_planeMap[1]);
}


BasicShapes::~BasicShapes()
{
}

bool BasicShapes::LoadT8Map(char *fileName, GLuint &texture)
{
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	unsigned char *data;	  // Actual RGB data

	FILE *file;

	fopen_s(&file, fileName, "rb");

	if (!file)
	{
		OutputDebugString(L"Cannot read file\n");
		return false;
	}
	if (fread(header, 1, 54, file) != 54)
	{
		OutputDebugString(L"Not 54 bytes. Error\n");
		return false;
	}
	if (header[0] != 'B' || header[1] != 'M') {
		OutputDebugString(L"Not a correct BMP file\n");
		return 0;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);

	// Create one OpenGL texture
	glGenTextures(1, &texture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Give the image to OpenGL

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	delete data;
	OutputDebugString(L"Read file successful\n");
	return true;
										 
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 4, )
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

void BasicShapes::Airplane(float x, float y, float z)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(-angle, 0.0, 1.0, 0.0);
	glTranslatef(30, 0, 0);
	glRotatef(30, 0.0, 0.0, 1.0);
	//=============================================
	glPushMatrix();//
	glRotatef(-angle * 30, 0.0, 0.0, 1.0);
	glColor3f(0.0, 0.0, 1.0);
	Box(1.0f, 0.1f, 0.02f);
	glPopMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_planeMap[1]);
	glTranslatef(0.0f, 0.0f, -0.5f);
	gluSphere(m_quadricObj, 0.4f, 8, 8);
	//=============================================
	glTranslatef(0.0f, -0.0f, -2);
	gluCylinder(m_quadricObj, 0.4, 0.4, 2.0, 8, 4);
	//=====================================================
	glRotatef(-180, 1.0, 0.0, 0.0);
	glTranslatef(0.0f, -0.0f, 0.0f);
	gluCylinder(m_quadricObj, 0.4, 0.1, 1.5, 8, 4);
	//======================================================
	glBindTexture(GL_TEXTURE_2D, m_planeMap[0]);//
	glTranslatef(0.0f, -0.8f, 1.2f);
	Box(1.0, 0.05f, 0.3f);
	glTranslatef(0.0f, 0.1f, 0.0f);
	Box(0.05f, 0.6f, 0.30f);
	//======================================================
	glTranslatef(0.0f, 0.7f, -1.9f);
	Box(3, 0.05f, 0.5f);
	//======================================================
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	angle += 0.1f;
	if (angle > 360) angle = 0;
}

void BasicShapes::Box(float x, float y, float z)
{
	glPushMatrix();
	glScalef(x, y, z);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);//
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);//
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);//
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);//
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);//
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);//
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}