/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Camera.cpp
Purpose: Making camera for showing object as a view space
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Camera.hpp"
#include "Application.hpp"

Camera::Camera()
{
	width = 800.0f;
	height = 600.0f;
	position = glm::vec3{ 0,0,0};
	near = 0.1f;
	far = 100.0f;
	fov = glm::radians(45.0f);

	back = glm::vec3(0, 0, 1);
	right = cross(-back, glm::vec3{0,1,0});
	up = cross(back, right);

	originUp = up;
	originBack = back;
	originRight = right;
	originPosition = position;
}

Camera::Camera(glm::vec3 eye, glm::vec3 look)
{
	width = 800.0f;
	height = 600.0f;
	position = eye;
	near = 0.1f;
	far = 100.0f;
	fov = glm::radians(45.0f);

	back = -look;
	right = cross(-back, glm::vec3{ 0,1,0 });
	up = cross(back, right);

	originUp = up;
	originBack = back;
	originRight = right;
	originPosition = position;
}

void Camera::Initialize()
{
	const auto windowSize = APPLICATION->GetWindowSize();
	width = windowSize.x;
	height = windowSize.y;
}

void Camera::Update()
{
}

void Camera::Delete()
{
}


glm::mat4 Camera::GetView() const
{
	auto look = -back;
	return lookAt(position, position + look, up);
}

glm::mat4 Camera::GetProjection() const
{
	auto ratio = width / height;
	return glm::perspective(fov, ratio, near, far);
}

glm::vec3 Camera::GetLook() const
{
	return -back;
}

glm::vec3 Camera::GetPosition() const
{
	return position;
}

void Camera::SetOriginData(CameraOriginData data)
{
	position = data.position;

	switch (data.rotate)
	{
	case CameraRotate::Pitch:
		SetPitch(data.degree);
		break;
	case CameraRotate::Yaw:
		SetYaw(data.degree);
		break;
	case CameraRotate::Roll:
		SetRoll(data.degree);
		break;
	}

	originPosition = data.position;
	originUp = up;
	originBack = back;
	originRight = right;
}

void Camera::SetFOV(const float fov)
{
	this->fov = fov;
}

void Camera::SetNear(const float near)
{
	this->near = near;
}

void Camera::SetFar(const float far)
{
	this->far = far;
}

void Camera::SetPosition(glm::vec3 position)
{
	this->position = position;
}

void Camera::SetViewSize(glm::vec2 windowSize)
{
	width = windowSize.x;
	height = windowSize.y;
}

void Camera::Move(float speed, CameraMoveAxis axis)
{
	if(axis == CameraMoveAxis::Front)
		position = position - (back * speed);
	if(axis == CameraMoveAxis::Back)
		position = position + (back * speed);
	if (axis == CameraMoveAxis::Right)
		position = position + (right * speed);
	if (axis == CameraMoveAxis::Left)
		position = position - (right * speed);
}


void Camera::SetPitch(float degree)
{
	glm::mat4 rotationMatrix = rotate(glm::radians(degree), right);
	glm::vec4 tempBack = rotationMatrix * glm::vec4(back, 1);
	glm::vec4 tempUp = rotationMatrix * glm::vec4(up, 1);

	back = {tempBack.x, tempBack.y, tempBack.z};
	up = {tempUp.x, tempUp.y, tempUp.z};
}

void Camera::SetYaw(float degree)
{
	glm::mat4 rotationMatrix = rotate(glm::radians(degree), up);
	glm::vec4 tempRight = rotationMatrix * glm::vec4(right, 1);
	glm::vec4 tempBack = rotationMatrix * glm::vec4(back, 1);

	right= { tempRight.x, tempRight.y, tempRight.z };
	back = { tempBack.x, tempBack.y, tempBack.z };
}

void Camera::SetRoll(float degree)
{
	glm::mat4 rotationMatrix = rotate(glm::radians(degree), back);
	glm::vec4 tempUp = rotationMatrix * glm::vec4(up, 1);
	glm::vec4 tempRight = rotationMatrix * glm::vec4(right, 1);

	up = { tempUp.x, tempUp.y, tempUp.z };
	right = { tempRight.x, tempRight.y, tempRight.z };
}

void Camera::ResetPosition()
{
	position = originPosition;
	up = originUp;
	back = originBack;
	right = originRight;
}

