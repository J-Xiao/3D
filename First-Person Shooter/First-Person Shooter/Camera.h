#pragma once

#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define VK_W 0x57
#define VK_S 0x53
#define VK_A 0x41
#define VK_D 0x44
#define RANDF (((float)rand() - (float)rand()) / RAND_MAX)
#define MAP_W 32
#define MAP_SCALE 24.0f
#define MAP MAP_W*MAP_SCALE/2
#define BITMAP_ID 0x4D42

struct Point
{
	double x;
	double y;
	double z;
};

class Camera
{
public:
	Camera();
	virtual ~Camera();

	BOOL DisplayScene();
	GLvoid DrawGround();
	void DrawTerrain();
	void CreateSkyBox(int a, int wi, int he, int le);
	void ShowTree0(float x, float z, float h, float s, int texIndex);

private:
	void InitTerrain(float h);
	float GetHeight(float x, float z);

	BITMAPINFOHEADER  m_bit;
	GLuint m_texture[16];
	unsigned char *m_imageData;
	unsigned char *LoadBit(char *fileName, BITMAPINFOHEADER *bitmap);
	void texture(UINT texture);
	bool LoadT8Map(char *fileName, GLuint &texture);
	bool LoadT8Map(LPWSTR filename, GLuint &texture);
	void LoadT16(char *filename, GLuint &texture); 

	const float SPRINT_SPEED = 0.2f;
	const float NORMAL_SPEED = 0.1f;

	float m_speed; 
	float m_elev;
	Point m_cameraPos;
	Point m_targetPos;

	float m_elevationAngle;
	float m_directionAngle;
	float m_directionRadXZ;

	float m_terrain[MAP_W * MAP_W][3];
	int m_index[MAP_W * MAP_W * 2];
	float m_texCoord[MAP_W * MAP_W][2];
};