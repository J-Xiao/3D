#pragma once
class Camera
{
public:
	Camera();
	virtual ~Camera();

	BOOL DisplayScene();

private:
	float speed;
	float sprintSpeed;
	float normalSpeed;
};

