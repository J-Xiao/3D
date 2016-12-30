#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
	m_directionAngle = -90;

	m_cameraPos.x = 0;
	m_cameraPos.y = 1.8;
	m_cameraPos.z = 0;
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

	if (m_cameraPos.x < -MAP * 2 + 20)
		m_cameraPos.x = -MAP * 2 + 20;

	if (m_cameraPos.x > MAP * 2 - 20)
		m_cameraPos.x = MAP * 2 - 20;

	if (m_cameraPos.z < -MAP * 2 + 20)
		m_cameraPos.z = -MAP * 2 + 20;

	if (m_cameraPos.z > MAP * 2 - 20)
		m_cameraPos.z = MAP * 2 - 20;

	if(m_cameraPos.z )

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