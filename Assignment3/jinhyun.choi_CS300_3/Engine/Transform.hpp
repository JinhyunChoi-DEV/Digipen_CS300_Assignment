/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Transform.hpp
Purpose: Header to make Transform class
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once
#include <utility>
#include<glm/glm.hpp>
#include "Component.hpp"

enum RotateAxis
{
	X, Y, Z
};

class Transform : public Component
{
public:
	Transform();

	void Initialize() override;
	void Update() override;
	void Delete() override;

	void SetTranslate(glm::vec3 position);
	void SetScale(glm::vec3 scale);
	void SetRotation(float degree, RotateAxis axis);

	glm::mat4 GetMatrix() const;
	glm::mat4 GetNormalMatrix() const;
	glm::vec3 GetPosition() const { return position; }
	glm::vec3 GetScale() const { return scale; }
	std::pair<float, glm::vec3> GetRotate() const { return rotate; }

private:
	glm::vec3 position{ 0,0,0 };
	glm::vec3 scale{ 1,1,1 };
	std::pair<float, glm::vec3> rotate = { 0, {1,1,1} };
	glm::mat4 transform = glm::mat4(1.0f);

	glm::mat4 translate = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	glm::vec3 GetAxis(RotateAxis axis);
};
