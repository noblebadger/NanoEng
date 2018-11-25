#include "Camera.h"
#include <iostream>

Camera::Camera()
	: upDir{0,1,0}, pos{0.2,0.2,0.1}, target{0,0,0}, right{1,0,0}
{
	setUpViewProjection();
	moveCam('w');
}

void Camera::moveCam(char key)
{
	float spd = 0.2;
	if (key == 'a')
		pos -= spd * right;
	else if (key == 'd')
		pos += spd * right;
	else if (key == 's')
		pos -= spd * upDir;
	else if (key == 'w')
		pos += spd * upDir;
	view = glm::lookAt(pos, target, upDir);
	/* update mvp !*/
	mvp = proj*view*model;
}

Camera::~Camera()
{
}
