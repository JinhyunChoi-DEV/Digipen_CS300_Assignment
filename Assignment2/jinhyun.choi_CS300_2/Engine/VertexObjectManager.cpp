/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: VertexObjectManager.cpp
Purpose: Making of manager about Vertex Object like VAO, VBO, EBO.
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include <glad/glad.h>
#include "VertexObjectManager.hpp"
#include "MeshManager.hpp"
#include "Shader.hpp"

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

void VertexObjectManager::GenerateObject(std::string name, const Mesh* mesh)
{
	auto VAO = vertexArrays[name];
	auto VBO = vertexBuffers[name];
	auto EBO = elementBuffers[name];

	const auto positions = mesh->GetPositions();
	const auto vertexNormals = mesh->GetVertexNormals();
	const auto indices = mesh->GetIndices();
	const auto positionSize = positions.size();
	const auto vertexNormalSize = vertexNormals.size();
	const auto totalSize = positionSize + vertexNormalSize;
	const auto dataSize = sizeof(glm::vec3);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (totalSize * dataSize), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, (dataSize * positionSize), &positions[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (dataSize * vertexNormalSize), (dataSize * vertexNormalSize), &vertexNormals[0]);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(positionSize * dataSize));
	glEnableVertexAttribArray(1);

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
