#include "Light.hpp"

Light::Light() : direction(0),
	ambient(1.0f, 1.0f, 1.0f), diffuse(1.0f, 1.0f, 1.0f), specular(1.0f, 1.0f, 1.0f),
	type(LightType::Directional), cuttOff(0.0f), outerCutOff(0.0f),
	constant(1.0f), linear(0.09f), quadratic(0.032f),
	color(1.0f, 1.0f, 1.0f)
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

void Light::SetCutOff(float cutOff)
{
	this->cuttOff = cutOff;
}

void Light::SetOuterCutOff(float outerCutOff)
{
	this->outerCutOff = outerCutOff;
}

void Light::SetConstant(float constant)
{
	this->constant = constant;
}

void Light::SetLinear(float linear)
{
	this->linear = linear;
}

void Light::SetQuadratic(float quadratic)
{
	this->quadratic = quadratic;
}

void Light::SetColor(glm::vec3 color)
{
	this->color = color;
}
