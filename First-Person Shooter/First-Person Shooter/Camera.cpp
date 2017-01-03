#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
	m_directionAngle = -90;

	m_cameraPos.x = 0;
	m_cameraPos.z = 0;

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

	if (m_cameraPos.x < -MAP * 2 + 20)
		m_cameraPos.x = -MAP * 2 + 20;

	if (m_cameraPos.x > MAP * 2 - 20)
		m_cameraPos.x = MAP * 2 - 20;

	if (m_cameraPos.z < -MAP * 2 + 20)
		m_cameraPos.z = -MAP * 2 + 20;

	if (m_cameraPos.z > MAP * 2 - 20)
		m_cameraPos.z = MAP * 2 - 20;

	m_cameraPos.y = 1.8;

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
			m_terrain[vertex][1] = h + RANDF * h;
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
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	for (int z = 0; z < MAP_W - 1; ++z)
	{
		glDrawElements(GL_LINE_STRIP, MAP_W * 2, GL_UNSIGNED_INT, &m_index[z * MAP_W * 2]);
	}
}