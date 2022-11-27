/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Camera.hpp
Purpose: Header of baseCamera class
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include "Component.hpp"

enum class CameraMoveAxis
{
	Front, Back, Right, Left
};

enum CameraRotate
{
	Pitch, Yaw, Roll
};

struct CameraOriginData
{
	glm::vec3 position;
	CameraRotate rotate;
	float degree;
};

class Camera : public Component
{
public:
	Camera();
	Camera(glm::vec3 eye, glm::vec3 look);

	void Initialize() override;
	void Update() override;
	void Delete() override;

	glm::mat4 GetView() const;
	glm::mat4 GetProjection() const;
	glm::vec3 GetLook() const;
	glm::vec3 GetPosition() const;
	float GetNear() const { return near; }
	float GetFar() const { return far; }

	void SetOriginData(CameraOriginData data);
	void SetFOV(float fov);
	void SetNear(float near);
	void SetFar(float far);
	void SetPosition(glm::vec3 position);
	void SetViewSize(glm::vec2 windowSize, bool isConstRatio = false);
	void Move(float speed, CameraMoveAxis axis);
	void SetPitch(float degree);
	void SetYaw(float degree);
	void SetRoll(float degree);
	void ResetPosition();
	void SetRatio(float value);

private:
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 back;

	glm::vec3 originPosition;
	glm::vec3 originUp;
	glm::vec3 originRight;
	glm::vec3 originBack;

	float width, height, near, far, fov, ratio;
};