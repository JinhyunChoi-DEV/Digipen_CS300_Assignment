#pragma once
#include <utility>
#include <glm/glm.hpp>
#include "Component.hpp"

enum class LightType
{
	Directional, Point, Spotlight
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
	void SetCutOff(float cutOff);
	void SetOuterCutOff(float outerCutOff);
	void SetConstant(float constant);
	void SetLinear(float linear);
	void SetQuadratic(float quadratic);
	void SetColor(glm::vec3 color);	

	glm::vec3 GetDirection() const { return direction; }
	glm::vec3 GetAmientIntensity() const { return ambient; }
	glm::vec3 GetDiffuseIntensity() const { return diffuse; }
	glm::vec3 GetSpecularIntensity() const { return specular; }
	LightType GetType() const { return type; }
	float GetCutOffAngle() const { return cuttOff; }
	float GetOuterCutOffAngle() const { return outerCutOff; }
	float GetConstant() const { return constant; }
	float GetLinear() const { return linear; }
	float GetQuadratic() const { return quadratic; }
	glm::vec3 GetColor() const { return color; }

private:
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	LightType type;

	// Point && Spotlight 
	float cuttOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	glm::vec3 color;
};