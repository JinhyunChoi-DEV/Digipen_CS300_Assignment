/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: VertexObjectManager.cpp
Purpose: Making of manager about Vertex Object like VAO, VBO, EBO.
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

#include <glad/glad.h>
#include "VertexObjectManager.hpp"
#include "MeshManager.hpp"
#include "Shader.hpp"

VertexObjectManager* VERTEX_OBJECTS = nullptr;
VertexObjectManager::VertexObjectManager()
{
	if (VERTEX_OBJECTS == nullptr)
		VERTEX_OBJECTS = this;
}

unsigned VertexObjectManager::GetObjectVAO(const Mesh* mesh)
{
	auto name = mesh->GetName();
	auto VAO = vertexArrays[name];
	auto VBO = vertexBuffers[name];

	if (VAO != 0 && VBO != 0)
		return VAO;

	auto type = mesh->GetType();

	if (type == DrawType::Line)
		GenerateLines(name, mesh);
	else
		GenerateObject(name, mesh);

	return vertexArrays[name];
}

unsigned VertexObjectManager::GetVertexNormalVAO(const Mesh* mesh)
{
	auto name = mesh->GetName() + "_VertexNormal";
	auto VAO = vertexArrays[name];
	auto VBO = vertexBuffers[name];

	if (VAO != 0 || VBO != 0)
		return VAO;

	GenerateNormalLines(name, mesh, true);
	return vertexArrays[name];
}

unsigned VertexObjectManager::GetFaceNormalVAO(const Mesh* mesh)
{
	auto name = mesh->GetName() + "_FaceNormal";
	auto VAO = vertexArrays[name];
	auto VBO = vertexBuffers[name];

	if (VAO != 0 || VBO != 0)
		return VAO;

	GenerateNormalLines(name, mesh, false);
	return vertexArrays[name];
}

void VertexObjectManager::RebindTexture(const Mesh* mesh, const Texture* texture)
{
	auto entityType = texture->GetEntityType();
	auto mappingType = texture->GetMappingType();

	auto name = mesh->GetName();
	auto VAO = vertexArrays[name];
	auto VBO = vertexBuffers[name];

	const auto positions = mesh->GetPositions();
	const auto vertexNormals = mesh->GetVertexNormals();
	const auto indices = mesh->GetIndices();
	std::vector<glm::vec2> baseTextureCoordinates;
	if(entityType == TextureEntityType::VertexPosition)
		baseTextureCoordinates = mesh->GetTextureCoordinateWithPosition(mappingType);
	else if(entityType == TextureEntityType::VertexNormal)
		baseTextureCoordinates = mesh->GetTextureCoordinateWithVertexNormal(mappingType);

	const auto vec2DataSize = sizeof(glm::vec2);
	const auto vec3DataSize = sizeof(glm::vec3);

	const auto positionSize = positions.size();
	const auto vertexNormalSize = vertexNormals.size();
	const auto textureCoordinateSize = baseTextureCoordinates.size();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (positionSize * vec3DataSize + vertexNormalSize * vec3DataSize + textureCoordinateSize * vec2DataSize), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, (vec3DataSize * positionSize), &positions[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (vec3DataSize * positionSize), (vec3DataSize * vertexNormalSize), &vertexNormals[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (vec3DataSize * positionSize + vertexNormalSize * vec3DataSize), (vec2DataSize * textureCoordinateSize), &baseTextureCoordinates[0]);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(positionSize * vec3DataSize));
	glEnableVertexAttribArray(1);

	//texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(vec2DataSize * positionSize + vertexNormalSize * vec3DataSize));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void VertexObjectManager::GenerateObject(std::string name, const Mesh* mesh)
{
	auto VAO = vertexArrays[name];
	auto VBO = vertexBuffers[name];
	auto EBO = elementBuffers[name];

	const auto positions = mesh->GetPositions();
	const auto vertexNormals = mesh->GetVertexNormals();
	const auto indices = mesh->GetIndices();
	const auto baseTextureCoordinates = mesh->GetTextureCoordinateWithPosition(TextureMappingType::Cube);

	const auto positionSize = positions.size();
	const auto vertexNormalSize = vertexNormals.size();
	const auto textureCoordinateSize = baseTextureCoordinates.size();

	const auto vec2DataSize = sizeof(glm::vec2);
	const auto vec3DataSize = sizeof(glm::vec3);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (positionSize * vec3DataSize + vertexNormalSize * vec3DataSize + textureCoordinateSize * vec2DataSize), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, (vec3DataSize * positionSize), &positions[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (vec3DataSize * positionSize), (vec3DataSize * vertexNormalSize), &vertexNormals[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (vec3DataSize * positionSize + vertexNormalSize * vec3DataSize), (vec2DataSize * textureCoordinateSize), &baseTextureCoordinates[0]);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(positionSize * vec3DataSize));
	glEnableVertexAttribArray(1);

	//texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(vec3DataSize * positionSize + vertexNormalSize * vec3DataSize));
	glEnableVertexAttribArray(2);

	// ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	vertexArrays[name] = VAO;
	vertexBuffers[name] = VBO;
	elementBuffers[name] = EBO;
}

void VertexObjectManager::GenerateLines(std::string name, const Mesh* mesh)
{
	auto VAO = vertexArrays[name];
	auto VBO = vertexBuffers[name];

	const auto positions = mesh->GetPositions();
	const auto dataSize = sizeof(glm::vec3);
	const auto positionSize = positions.size();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (dataSize * positionSize), &positions[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	vertexArrays[name] = VAO;
	vertexBuffers[name] = VBO;
}

void VertexObjectManager::GenerateNormalLines(std::string name, const Mesh* mesh, bool isVertexNormal)
{
	auto VAO = vertexArrays[name];
	auto VBO = vertexBuffers[name];

	std::vector<glm::vec3> lines;

	if (isVertexNormal)
		lines = mesh->GetVertexNormalLines();
	else
		lines = mesh->GetFaceNormalLines();

	const auto dataSize = sizeof(glm::vec3);
	const auto positionSize = lines.size();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (dataSize * positionSize), &lines[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	vertexArrays[name] = VAO;
	vertexBuffers[name] = VBO;
}
