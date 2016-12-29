#pragma once

#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define VK_W 0x57
#define VK_S 0x53
#define MAP 40

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
	double m_cameraPos[3];
	double m_targetPos[3];

	float m_elevationAngle;
	float m_directionAngle;
	float m_directionRadXZ;
};

