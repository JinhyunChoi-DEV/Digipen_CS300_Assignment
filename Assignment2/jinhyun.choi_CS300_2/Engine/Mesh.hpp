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
#include "Texture.hpp"

enum class DrawType
{
	Line, Solid, ObjectModel, Light
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
	void SetColor(glm::vec3 color);
	void SetDrawType(DrawType type);

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
	glm::vec3 GetColor() const { return objectColor; }
	std::vector<glm::vec2> GetTextureCoordinateWithPosition(TextureMappingType type) const;
	std::vector<glm::vec2> GetTextureCoordinateWithVertexNormal(TextureMappingType type) const;
	glm::vec3 GetMinVertex() const { return minVertex; }
	glm::vec3 GetMaxVertex() const { return maxVertex; }
	glm::vec3 GetMinVertexNormal() const { return minVertexNormal; }
	glm::vec3 GetMaxVertexNormal() const { return maxVertexNormal; }

private:
	void CreateVertexNormalLines();
	void CreateFaceNormalLines();
	void CreateTextureCoordinates();
	void CreateMinMax();

	bool isMultipleFaceIndex = false;
	std::string name;
	Shader* shader = nullptr;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> vertexNormals;
	std::vector<glm::vec3> faceNormal;
	std::vector<glm::vec3> vertexNormalLines;
	std::vector<glm::vec3> faceNormalLines;

	std::vector<glm::vec2> planarTextureCoordinate;
	std::vector<glm::vec2> cylindricalTextureCoordinate;
	std::vector<glm::vec2> sphericalTextureCoordinate;
	std::vector<glm::vec2> cubeTextureCoordinate;

	std::vector<glm::vec2> planarTextureCoordinateWithVertexNormal;
	std::vector<glm::vec2> cylindricalTextureCoordinateWithVertexNormal;
	std::vector<glm::vec2> sphericalTextureCoordinateWithVertexNormal;
	std::vector<glm::vec2> cubeTextureCoordinateWithVertexNormal;

	std::vector<unsigned int> indices;
	glm::vec3 minVertex;
	glm::vec3 maxVertex;
	glm::vec3 minVertexNormal;
	glm::vec3 maxVertexNormal;
	glm::vec3 objectColor;

	DrawType type;
};
