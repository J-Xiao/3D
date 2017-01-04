#include "stdafx.h"
#include "Camera.h"
#include "bmpread.h"

Camera::Camera()
{
	m_directionAngle = -90;

	m_cameraPos.x = 0;
	m_cameraPos.z = 0;

	m_imageData = LoadBit("data/images/Terrain1.bmp", &m_bit);
	LoadT8Map("data/images/sand0.bmp", m_texture[0]);
	InitTerrain(1);
}


Camera::~Camera()
{
}

BOOL Camera::DisplayScene()
{
	if (KEY_DOWN(VK_SHIFT))
	{
		m_speed = SPRINT_SPEED;
	}
	else
	{
		m_speed = NORMAL_SPEED;
	}

	m_directionRadXZ = float(3.14 * m_directionAngle / 180);

	if (KEY_DOWN(VK_W))
	{
		m_cameraPos.x += cos(m_directionRadXZ) * m_speed;
		m_cameraPos.z += sin(m_directionRadXZ) * m_speed;
	}

	if (KEY_DOWN(VK_S))
	{
		m_cameraPos.x -= cos(m_directionRadXZ) * m_speed;
		m_cameraPos.z -= sin(m_directionRadXZ) * m_speed;
	}

	if (KEY_DOWN(VK_A))
	{
		m_cameraPos.x += sin(m_directionRadXZ) * m_speed;
		m_cameraPos.z -= cos(m_directionRadXZ) * m_speed;
	}

	if (KEY_DOWN(VK_D))
	{
		m_cameraPos.x -= sin(m_directionRadXZ) * m_speed;
		m_cameraPos.z += cos(m_directionRadXZ) * m_speed;
	}


	if (m_cameraPos.x < MAP_SCALE)
		m_cameraPos.x = MAP_SCALE;

	if (m_cameraPos.x > (MAP_W - 2) * MAP_SCALE)
		m_cameraPos.x = (MAP_W - 2) * MAP_SCALE;

	if (m_cameraPos.z < -(MAP_W - 2) * MAP_SCALE)
		m_cameraPos.z = -(MAP_W - 2) * MAP_SCALE;

	if (m_cameraPos.z > -MAP_SCALE)
		m_cameraPos.z = -MAP_SCALE;

	m_cameraPos.y = 1.8 + GetHeight((float) m_cameraPos.x, (float) m_cameraPos.z);

	m_targetPos.x = m_cameraPos.x + cos(m_directionRadXZ);
	m_targetPos.y = m_cameraPos.y;
	m_targetPos.z = m_cameraPos.z + sin(m_directionRadXZ);

	gluLookAt(
		m_cameraPos.x, m_cameraPos.y, m_cameraPos.z,
		m_targetPos.x, m_targetPos.y, m_targetPos.z,
		0.0, 1.0, 0.0
	);

	return TRUE;
} 

GLvoid Camera::DrawGround()
{
	glPushMatrix();
	glColor3f(0.5f, 0.7f, 1.0f);
	glTranslatef(0, 0, 0);
	int size = (int)(MAP * 2);

	glBegin(GL_LINES);
	for (int i = -size; i < size; i += 4)
	{
		glVertex3i(i, 0, -size);
		glVertex3i(i, 0, size);
		glVertex3i(-size, 0, i);
		glVertex3i(size, 0, i);
	}
	glEnd();

	glPopMatrix();
}

void Camera::InitTerrain(float h)
{
	int index = 0;

	for (int z = 0; z < MAP_W; z++)
	{
		for (int x = 0; x < MAP_W; x++)
		{
			int vertex = MAP_W * z + x;

			m_terrain[vertex][0] = float(x) * MAP_SCALE;
			m_terrain[vertex][1] = float(m_imageData[(z * MAP_W + x) * 3] / 3);
			m_terrain[vertex][2] = -float(z) * MAP_SCALE;

			m_texCoord[vertex][0] = (float)x;
			m_texCoord[vertex][1] = (float)z;

			m_index[index++] = vertex;
			m_index[index++] = vertex + MAP_W;
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, m_terrain);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, m_texCoord);
}

void Camera::DrawTerrain()
{
	glBindTexture(GL_TEXTURE_2D, m_texture[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	for (int z = 0; z < MAP_W - 1; ++z)
	{
		glDrawElements(GL_TRIANGLE_STRIP, MAP_W * 2, GL_UNSIGNED_INT, &m_index[z * MAP_W * 2]);
	}
}

float Camera::GetHeight(float x, float z)
{
	float CameraX = x / MAP_SCALE;
	float CameraZ = -z / MAP_SCALE;
	int Col0 = int(CameraX);
	int Row0 = int(CameraZ);
	int Col1 = Col0 + 1;
	int Row1 = Row0 + 1;

	if (Col1 > MAP_W)
		Col1 = 0;

	if (Row1 > MAP_W)
		Row1 = 0;

	float h00 = m_terrain[Col0 + Row0*MAP_W][1];
	float h01 = m_terrain[Col1 + Row0*MAP_W][1];
	float h11 = m_terrain[Col1 + Row1*MAP_W][1];
	float h10 = m_terrain[Col0 + Row1*MAP_W][1];

	float tx = CameraX - int(CameraX);
	float ty = CameraZ - int(CameraZ);
	float txty = tx * ty;

	return h00*(1.0f - ty - tx + txty)
		+ h01*(tx - txty)
		+ h11*txty
		+ h10*(ty - txty);
}

unsigned char * Camera::LoadBit(char *fileName, BITMAPINFOHEADER *bitmap)
{
	FILE *filePtr;
	BITMAPFILEHEADER header;
	unsigned char *image;

	fopen_s(&filePtr, fileName, "rb");

	if (filePtr == NULL)
		return NULL;

	fread(&header, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (header.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	fread(bitmap, sizeof(BITMAPFILEHEADER), 1, filePtr);
	fseek(filePtr, header.bfOffBits, SEEK_SET);
	image = (unsigned char*)malloc(bitmap->biSizeImage);

	if (!image)
	{
		free(image);
		fclose(filePtr);
		return NULL;
	}

	fread(image, 1, bitmap->biSizeImage, filePtr);

	if (image == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	for (int imageIndex = 0; imageIndex < bitmap->biSizeImage; imageIndex += 3)
	{
		unsigned char tempRGB = image[imageIndex];
		image[imageIndex] = image[imageIndex + 2];
		image[imageIndex + 2] = tempRGB;
	}

	fclose(filePtr);
	return image;
}

bool Camera::LoadT8Map(char *filename, GLuint &texture)
{
	bmpread_t image;
	
	if (!bmpread(filename, 0, &image))
	{
		return false;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width, image.height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, image.rgb_data);

	bmpread_free(&image);
	return true;
}