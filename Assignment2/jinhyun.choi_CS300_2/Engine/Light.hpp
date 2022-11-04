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
	/*
	void SetConstant(float constant);
	void SetLinear(float linear);
	void SetQuadratic(float quadratic);
	*/
	void SetFallOut(float fallOut);
	void SetColor(glm::vec3 color);

	glm::vec3 GetDirection() const { return direction; }
	glm::vec3 GetAmientIntensity() const { return ambient; }
	glm::vec3 GetDiffuseIntensity() const { return diffuse; }
	glm::vec3 GetSpecularIntensity() const { return specular; }
	LightType GetType() const { return type; }
	float GetInnerAngle() const { return innerAngle; }
	float GetOuterAngle() const { return outerAngle; }
	/*float GetConstant() const { return constant; }
	float GetLinear() const { return linear; }
	float GetQuadratic() const { return quadratic; }*/
	float GetFallOut() const { return fallOut; }

private:
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	LightType type;

	// Point && Spotlight 
	float innerAngle;
	float outerAngle;

	/*float constant;
	float linear;
	float quadratic;*/

	float fallOut;
};