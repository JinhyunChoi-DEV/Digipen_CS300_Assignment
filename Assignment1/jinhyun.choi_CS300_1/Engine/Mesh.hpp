/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.hpp
Purpose: Header of Mesh class
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Component.hpp"

enum DrawType
{
	Line,
	Solid,
	ObjectModel
};

struct ObjectVertex
{
	glm::vec3 position;
	glm::vec3 normal;
};

struct LineVertex
{
	glm::vec3 position;
};

class Shader;

class Mesh : public Component
{
public:
	Mesh() = default;
	Mesh(std::string name, std::vector<glm::vec3> positions, std::vector<glm::vec3> vertexNormal,
		std::vector<glm::vec3> faceNormal, std::vector<unsigned int> indices, DrawType type);
	Mesh(std::string name, std::vector<glm::vec3> positions, DrawType type);
	Mesh(const Mesh* mesh);

	void Initialize() override;
	void Update() override;
	void Delete() override;
	void SetShader(std::string name);
	void SetMultipleFaceIndex(bool flag);

	Shader* GetShader() const { return shader; }
	DrawType GetType() const { return type; }
	bool GetIsMultipleFaceIndex() const { return isMultipleFaceIndex; }
	std::string GetName() const { return name; }
	std::vector<glm::vec3> GetPositions() const { return positions; }
	std::vector<glm::vec3> GetVertexNormals() const { return vertexNormals; }
	std::vector<glm::vec3> GetFaceNormal() const { return faceNormal; }
	std::vector<unsigned int> GetIndices() const { return indices; }
	std::vector<glm::vec3> GetVertexNormalLines() const { return vertexNormalLines; }
	std::vector<glm::vec3> GetFaceNormalLines() const { return faceNormalLines; }

private:
	void CreateVertexNormalLines();
	void CreateFaceNormalLines();

	bool isMultipleFaceIndex = false;
	std::string name;
	Shader* shader = nullptr;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> vertexNormals;
	std::vector<glm::vec3> faceNormal;
	std::vector<glm::vec3> vertexNormalLines;
	std::vector<glm::vec3> faceNormalLines;
	std::vector<unsigned int> indices;
	DrawType type;
};
