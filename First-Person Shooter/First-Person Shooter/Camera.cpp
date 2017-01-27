#include "stdafx.h"
#include "Camera.h"
#include "stb_image_aug.h"

Camera::Camera()
{
	//TCHAR NPath[MAX_PATH];
	//GetCurrentDirectory(MAX_PATH, NPath);

	m_directionAngle = -90;

	m_cameraPos.x = 0;
	m_cameraPos.z = 0;
	m_elevationAngle = 0;

	m_imageData = LoadBit("data/images/Terrain1.bmp", &m_bit);
	LoadT8Map("data/images/sand0.bmp", m_texture[0]);
	LoadT8Map("data/images/0RBack.bmp", m_texture[2]);
	LoadT8Map("data/images/0Front.bmp", m_texture[3]);
	LoadT8Map("data/images/0Top.bmp", m_texture[4]);
	LoadT8Map("data/images/0Left.bmp", m_texture[5]);
	LoadT8Map("data/images/0Right.bmp", m_texture[6]);
	InitTerrain(1);
	glEnable(GL_TEXTURE_2D);
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

	if (KEY_DOWN(VK_UP))
		m_elevationAngle += 0.1f;

	if (KEY_DOWN(VK_DOWN))
		m_elevationAngle -= 0.1f;

	if (KEY_DOWN(VK_LEFT))
		m_directionAngle -= 5;

	if (KEY_DOWN(VK_RIGHT))
		m_directionAngle += 5;

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
	m_targetPos.y = m_cameraPos.y + sin(m_elevationAngle);
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
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	for (int z = 0; z < MAP_W - 1; z++)
		glDrawElements(GL_TRIANGLE_STRIP, MAP_W * 2, GL_UNSIGNED_INT, &m_index[z*MAP_W * 2]);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
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

	fread(bitmap, sizeof(BITMAPINFOHEADER), 1, filePtr);
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
	int width, height, numComponents;

	unsigned char *imageData = stbi_load(filename, &width, &height, &numComponents, 4);

	if (imageData == NULL)
		return false;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);


	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGB, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);

	return true;
}

bool Camera::LoadT8Map(LPWSTR filename, GLuint &texture)
{
	BITMAPINFO BMInfo;								// need the current OpenGL device contexts in order to make use of windows DIB utilities  
	const HDC gldc = wglGetCurrentDC();   			// a handle for the current OpenGL Device Contexts
													// assume there are errors until file is loaded successfully into memory  
	bool NoErrors = false;  								// release old data since this object could be used to load multiple Textures  
	//if (data != NULL) delete data;					// windows needs this info to determine what header info we are looking for  
	BMInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  // Get windows to determine color bit depth in the file for us  
	BMInfo.bmiHeader.biBitCount = 0;				// Get windows to open and load the BMP file and handle the messy decompression if the file is compressed  
													// assume perfect world and no errors in reading file, Ha Ha  
	HANDLE DIBHandle = LoadImage(0, filename, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);  // use windows to get header info of bitmap - assume no errors in header format 

	GetDIBits(gldc, (HBITMAP)DIBHandle, 0, 0, NULL, &BMInfo, DIB_RGB_COLORS);
	DWORD sizeX = BMInfo.bmiHeader.biWidth;
	DWORD sizeY = BMInfo.bmiHeader.biHeight;				// change color depth to 24 bits (3 bytes (BGR) / pixel)  
	BMInfo.bmiHeader.biBitCount = 24;				// don't want the data compressed  
	BMInfo.bmiHeader.biCompression = BI_RGB;
	const DWORD BitmapLength = sizeX * sizeY * 3;	// 3 bytes (BGR) per pixel (24bp)  
													// allocate enough memory to hold the pixel data in client memory  
	BYTE *data = new BYTE[BitmapLength];					// Get windows to do the dirty work of converting the BMP into the format needed by OpenGL  
													// if file is already 24 bit color then this is a waste of time but makes for short code  
													// Get the actual Texel data from the BMP object  

	if (GetDIBits(gldc, (HBITMAP)DIBHandle, 0, sizeY, data, &BMInfo, DIB_RGB_COLORS))
	{
		NoErrors = true;
		
		// NOTE: BMP is in BGR format but OpenGL needs RGB unless you use GL_BGR_EXT
		const DWORD BitmapLength = sizeX * sizeY * 3;
		BYTE Temp;  // not quick but it works  
		for (DWORD i = 0; i< BitmapLength; i += 3)
		{
			Temp = data[i];
			data[i] = data[i + 2];
			data[i + 2] = Temp;
		}
	}

	DeleteObject(DIBHandle);						// don't need the BMP Object anymore  
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, sizeX,
		sizeY, GL_RGB, GL_UNSIGNED_BYTE, data);

	delete data;

	return NoErrors;
}

void Camera::CreateSkyBox(int a, int wi, int he, int le)
{
	float width = MAP*wi;
	float height = MAP*he;
	float length = MAP*le;
	float x = MAP - width / 2;
	float y = MAP / a - height / 2;
	float z = -MAP - length / 2;
	///////////////////////////////////////////////////////////////////////////////
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y, z);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, m_texture[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, m_texture[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, m_texture[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, m_texture[6]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
