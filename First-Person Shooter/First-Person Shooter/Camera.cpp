#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
	sprintSpeed = 0.2f;
	normalSpeed = 0.1f;
}


Camera::~Camera()
{
}

BOOL Camera::DisplayScene()
{
	if (GetAsyncKeyState(VK_SHIFT))
	{
		speed = sprintSpeed;
	}
	else
	{
		speed = normalSpeed;
	}



	return TRUE;
}