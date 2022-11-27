/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Texture.cpp
Purpose: the cpp file for Texture data and functions
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

#include <glad/glad.h>
#include "Texture.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "TextureManager.hpp"

void Texture::Bind(const Mesh* mesh)
{
	auto shader = mesh->GetShader();
	auto minVertex = mesh->GetMinVertex();
	auto maxVertex = mesh->GetMaxVertex();
	auto minVertexNormal = mesh->GetMinVertexNormal();
	auto maxVertexNormal = mesh->GetMaxVertexNormal();

	// ambient
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, ambientTexture);
	shader->Set("ambientTexture", 10);

	// specular
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, specularTexture);
	shader->Set("specularTexture", 11);

	// diffuse
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, diffuseTexture);
	shader->Set("diffuseTexture", 12);

	// emissive
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, emissiveTexture);
	shader->Set("emissiveTexture", 13);

	shader->Set("useAmbientTexture", ambientTexture != -1);
	shader->Set("useSpecularTexture", specularTexture != -1);
	shader->Set("useDiffuseTexture", diffuseTexture != -1);
	shader->Set("useEmissiveTexture", emissiveTexture != -1);

	unsigned mapping = static_cast<std::underlying_type<TextureMappingType>::type>(mappingType);
	unsigned entity = static_cast<std::underlying_type<TextureEntityType>::type>(entityType);
	shader->Set("useTexture", true);
	shader->Set("mappingType", mapping);
	shader->Set("entityType", entity);
	shader->Set("gpuMode", isGPU);

	shader->Set("minVertex", minVertex);
	shader->Set("maxVertex", maxVertex);
	shader->Set("minVertexNormal", minVertexNormal);
	shader->Set("maxVertexNormal", maxVertexNormal);

	shader->Set("ambientColor", ambientColor);
	shader->Set("specularColor", specularColor);
	shader->Set("diffuseColor", diffuseColor);
	shader->Set("emissiveColor", emissiveColor);
}

void Texture::SetAmbient(std::string name)
{
	auto ppm = TEXTURES->Get(name);
	ambientTexture = ppm.GetBufferObject();
}

void Texture::SetSpecular(std::string name)
{
	auto ppm = TEXTURES->Get(name);
	specularTexture = ppm.GetBufferObject();
}

void Texture::SetDiffuse(std::string name)
{
	auto ppm = TEXTURES->Get(name);
	diffuseTexture = ppm.GetBufferObject();
}

void Texture::SetUseGPU(bool isGPU)
{
	this->isGPU = isGPU;
}

void Texture::SetMappingType(TextureMappingType type)
{
	mappingType = type;
}

void Texture::SetEntityType(TextureEntityType type)
{
	entityType = type;
}

void Texture::SetAmbientColor(glm::vec3 color)
{
	ambientColor = color;
}

void Texture::SetSpecularColor(glm::vec3 color)
{
	specularColor = color;
}

void Texture::SetDiffuseColor(glm::vec3 color)
{
	diffuseColor = color;
}

void Texture::SetEmissiveColor(glm::vec3 color)
{
	emissiveColor = color;
}

void Texture::Initialize()
{}

void Texture::Update()
{}

void Texture::Delete()
{
}
