#pragma once
#include <string>
#include <glm/vec3.hpp>

#include "Component.hpp"


enum class TextureMappingType
{
	Planar = 0, Cylindrical, Spherical, Cube, Count
};

enum class TextureEntityType
{
	VertexPosition = 0, VertexNormal, Count
};


class Mesh;
class Texture : public Component
{
public:
	Texture() = default;

	void Bind(const Mesh* mesh);

	void SetAmbient(std::string name);
	void SetSpecular(std::string name);
	void SetDiffuse(std::string name);
	void SetUseGPU(bool isGPU);
	void SetMappingType(TextureMappingType type);
	void SetEntityType(TextureEntityType type);
	void SetAmbientColor(glm::vec3 color);
	void SetSpecularColor(glm::vec3 color);
	void SetDiffuseColor(glm::vec3 color);
	void SetEmissiveColor(glm::vec3 color);

	TextureMappingType GetMappingType() const { return mappingType; }
	TextureEntityType GetEntityType() const { return entityType; }
	bool IsGPUMode() const { return isGPU; }
	glm::vec3 GetAmbientColor() const { return ambientColor; }
	glm::vec3 GetSpecularColor() const { return specularColor; }
	glm::vec3 GetDiffuseColor() const { return diffuseColor; }
	glm::vec3 GetEmissiveColor() const { return emissiveColor; }

	void Initialize() override;
	void Update() override;
	void Delete() override;

private:
	TextureMappingType mappingType = TextureMappingType::Planar;
	TextureEntityType entityType = TextureEntityType::VertexPosition;
	bool isGPU = true;
	glm::vec3 ambientColor;
	glm::vec3 specularColor;
	glm::vec3 diffuseColor;
	glm::vec3 emissiveColor;

	int ambientTexture = -1;
	int specularTexture = -1;
	int diffuseTexture = -1;
	int emissiveTexture = -1;
};
