#pragma once

#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define VK_W 0x57
#define VK_S 0x53
#define VK_A 0x41
#define VK_D 0x44
#define MAP 40

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

private:
	const float SPRINT_SPEED = 0.2f;
	const float NORMAL_SPEED = 0.1f;

	float m_speed;
	Point m_cameraPos;
	Point m_targetPos;

	float m_elevationAngle;
	float m_directionAngle;
	float m_directionRadXZ;
};