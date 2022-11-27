/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Light.hpp
Purpose: the header file of light information and functions
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

#pragma once
#include <glm/glm.hpp>
#include "Component.hpp"

enum class LightType
{
	Directional = 0, Point, Spotlight, Count
};

class Light : public Component
{
public:
	Light();

	void Initialize() override;
	void Update() override;
	void Delete() override;

	void SetDirection(glm::vec3 direction);
	void SetAmientIntensity(glm::vec3 ambient);
	void SetDiffuseIntensity(glm::vec3 diffuse);
	void SetSpecularIntensity(glm::vec3 specular);
	void SetType(LightType type);
	void SetInnerAngle(float innerAngle);
	void SetOuterAngle(float outerAngle);
	void SetFallOut(float fallOut);
	void SetColor(glm::vec3 color);

	glm::vec3 GetDirection() const { return direction; }
	glm::vec3 GetAmientIntensity() const { return ambient; }
	glm::vec3 GetDiffuseIntensity() const { return diffuse; }
	glm::vec3 GetSpecularIntensity() const { return specular; }
	LightType GetType() const { return type; }
	float GetInnerAngle() const { return innerAngle; }
	float GetOuterAngle() const { return outerAngle; }
	float GetFallOut() const { return fallOut; }

private:
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	LightType type;
	float innerAngle;
	float outerAngle;
	float fallOut;
};