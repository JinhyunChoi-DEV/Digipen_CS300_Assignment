/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Transform.cpp
Purpose: Making Transform of object for translate, rotate, and scale
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include "Transform.hpp"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Transform::Transform()
{
}

void Transform::Initialize()
{
}

void Transform::Update()
{
}

void Transform::Delete()
{
}

void Transform::SetTranslate(glm::vec3 position)
{
	glm::mat4 tmp = glm::mat4(1.0f);

	this->position = position;
	translate = glm::translate(tmp, position);
}

void Transform::SetScale(glm::vec3 scale)
{
	glm::mat4 tmp = glm::mat4(1.0f);

	this->scale = scale;
	scaleMatrix = glm::scale(tmp, scale);
}

void Transform::SetRotation(float degree, RotateAxis axis)
{
	glm::mat4 tmp = glm::mat4(1.0f);
	const auto v = GetAxis(axis);

	rotationMatrix = glm::rotate(tmp, glm::radians(degree), v);
	this->rotate = std::make_pair(degree, v);

}

glm::mat4 Transform::GetMatrix() const
{
	return translate * rotationMatrix * scaleMatrix;
}

glm::mat4 Transform::GetNormalMatrix() const
{
	auto normalScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.7f));
	return translate * rotationMatrix * normalScale;
}

glm::vec3 Transform::GetAxis(RotateAxis axis)
{
	switch (axis)
	{
	case X:
		return glm::vec3{1,0,0};

	case Y:
		return glm::vec3{ 0,1,0 };

	case Z:
		return glm::vec3{ 0,0,1};

	default:
		return glm::vec3{ 0,0,0 };
	}
}
