/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Light.cpp
Purpose: the cpp file of light information and functions
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

#include "Light.hpp"

Light::Light() : direction(0),
ambient(1.0f, 1.0f, 1.0f), diffuse(1.0f, 1.0f, 1.0f), specular(1.0f, 1.0f, 1.0f),
type(LightType::Directional), innerAngle(0.0f), outerAngle(0.0f), fallOut(1.0f)
{ }

void Light::Initialize()
{ }

void Light::Update()
{ }

void Light::Delete()
{ }

void Light::SetDirection(glm::vec3 direction)
{
	this->direction = direction;
}

void Light::SetAmientIntensity(glm::vec3 ambient)
{
	this->ambient = ambient;
}

void Light::SetDiffuseIntensity(glm::vec3 diffuse)
{
	this->diffuse = diffuse;
}

void Light::SetSpecularIntensity(glm::vec3 specular)
{
	this->specular = specular;
}

void Light::SetType(LightType type)
{
	this->type = type;
}

void Light::SetInnerAngle(float innerAngle)
{
	this->innerAngle = innerAngle;
}

void Light::SetOuterAngle(float outerAngle)
{
	this->outerAngle = outerAngle;
}

//void Light::SetConstant(float constant)
//{
//	this->constant = constant;
//}
//
//void Light::SetLinear(float linear)
//{
//	this->linear = linear;
//}
//
//void Light::SetQuadratic(float quadratic)
//{
//	this->quadratic = quadratic;
//}

void Light::SetFallOut(float fallOut)
{
	this->fallOut = fallOut;
}

void Light::SetColor(glm::vec3 color)
{
	SetAmientIntensity(color);
	SetDiffuseIntensity(color);
	SetSpecularIntensity(color);
}
