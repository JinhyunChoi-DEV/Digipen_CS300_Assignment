/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.cpp
Purpose: Making a mesh for making object with holding model information like vertices, normals
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include <algorithm>
#include "Mesh.hpp"
#include "Graphic.hpp"
#include "MeshManager.hpp"
#include "Shader.hpp"
#include "UVGenerator.hpp"
#include "VertexHelper.hpp"

Mesh::Mesh(std::string name, std::vector<glm::vec3> positions, std::vector<glm::vec3> vertexNormal, 
	std::vector<glm::vec3> faceNormal, std::vector<unsigned int> indices,DrawType type)
	: name(name), positions(positions), vertexNormals(vertexNormal), faceNormal(faceNormal),indices(indices),
minVertex(0), maxVertex(0), minVertexNormal(0), maxVertexNormal(0),type(type)
{
	shader = GRAPHIC->GetShader("PhongShading");

	if(type != DrawType::Line)
	{
		CreateVertexNormalLines();
		CreateFaceNormalLines();
		CreateMinMax();
		CreateTextureCoordinates();
	}
}

Mesh::Mesh(std::string name, std::vector<glm::vec3> vertices, DrawType type)
	: name(name), positions(vertices), vertexNormals(), faceNormal(), indices(),
	minVertex(0), maxVertex(0), minVertexNormal(0), maxVertexNormal(0), type(type)
{
	shader = GRAPHIC->GetShader("Line");
}

Mesh::Mesh(const Mesh* mesh)
{
	name = mesh->GetName();
	isMultipleFaceIndex = mesh->GetIsMultipleFaceIndex();
	shader = mesh->GetShader();
	positions = mesh->GetPositions();
	vertexNormals = mesh->GetVertexNormals();
	faceNormal = mesh->GetFaceNormal();
	vertexNormalLines = mesh->GetVertexNormalLines();
	faceNormalLines = mesh->GetFaceNormalLines();
	planarTextureCoordinate = mesh->GetTextureCoordinateWithPosition(TextureMappingType::Planar);
	cylindricalTextureCoordinate = mesh->GetTextureCoordinateWithPosition(TextureMappingType::Cylindrical);
	sphericalTextureCoordinate = mesh->GetTextureCoordinateWithPosition(TextureMappingType::Spherical);
	cubeTextureCoordinate = mesh->GetTextureCoordinateWithPosition(TextureMappingType::Cube);
	planarTextureCoordinateWithVertexNormal = mesh->GetTextureCoordinateWithVertexNormal(TextureMappingType::Planar);
	cylindricalTextureCoordinateWithVertexNormal = mesh->GetTextureCoordinateWithVertexNormal(TextureMappingType::Cylindrical);
	sphericalTextureCoordinateWithVertexNormal = mesh->GetTextureCoordinateWithVertexNormal(TextureMappingType::Spherical);
	cubeTextureCoordinateWithVertexNormal = mesh->GetTextureCoordinateWithVertexNormal(TextureMappingType::Cube);
	indices = mesh->GetIndices();
	minVertex = mesh->GetMinVertex();
	maxVertex = mesh->GetMaxVertex();
	minVertexNormal = mesh->GetMinVertexNormal();
	maxVertexNormal = mesh->GetMaxVertexNormal();
	type = mesh->GetType();
}


void Mesh::Initialize()
{
}

void Mesh::Update()
{
}

void Mesh::Delete()
{
	shader = nullptr;
	positions.clear();
	faceNormal.clear();
	indices.clear();
}

void Mesh::SetShader(std::string name)
{
	shader = GRAPHIC->GetShader(name);
}

void Mesh::SetMultipleFaceIndex(bool flag)
{
	isMultipleFaceIndex = flag;
}

void Mesh::SetDrawType(DrawType type)
{
	this->type = type;
}

std::vector<glm::vec2> Mesh::GetTextureCoordinateWithPosition(TextureMappingType type) const
{
	if (type == TextureMappingType::Planar)
		return planarTextureCoordinate;

	if (type == TextureMappingType::Cylindrical)
		return cylindricalTextureCoordinate;

	if (type == TextureMappingType::Spherical)
		return sphericalTextureCoordinate;

	if (type == TextureMappingType::Cube)
		return cubeTextureCoordinate;

	return std::vector<glm::vec2>();
}

std::vector<glm::vec2> Mesh::GetTextureCoordinateWithVertexNormal(TextureMappingType type) const
{
	if (type == TextureMappingType::Planar)
		return planarTextureCoordinateWithVertexNormal;

	if (type == TextureMappingType::Cylindrical)
		return cylindricalTextureCoordinateWithVertexNormal;

	if (type == TextureMappingType::Spherical)
		return sphericalTextureCoordinateWithVertexNormal;

	if (type == TextureMappingType::Cube)
		return cubeTextureCoordinateWithVertexNormal;

	return std::vector<glm::vec2>();
}

void Mesh::CreateVertexNormalLines()
{
	for (unsigned i = 0; i < positions.size(); ++i)
	{
		auto v1 = positions[i];
		auto v2 = positions[i] + (vertexNormals[i] * 0.15f);
		vertexNormalLines.insert(vertexNormalLines.end(), { v1, v2});
	}
}

void Mesh::CreateFaceNormalLines()
{
	for(unsigned i=0; i< indices.size(); i+=3)
	{
		unsigned firstFactor = i;
		unsigned secondFactor = i + 1;
		unsigned thirdFactor = i + 2;

		int firstIndex = (int)indices[firstFactor];
		int secondIndex = (int)indices[secondFactor];
		int thirdIndex = (int)indices[thirdFactor];

		auto firstPoint = positions[firstIndex];
		auto secondPoint = positions[secondIndex];
		auto thirdPoint = positions[thirdIndex];

		auto v1 = (firstPoint + secondPoint + thirdPoint) / 3.0f;
		auto v2 = v1 + (faceNormal[i / 3] * 0.15f);

		faceNormalLines.insert(faceNormalLines.end(), { v1, v2});
	}
}

void Mesh::CreateTextureCoordinates()
{
	for(auto position : positions)
	{
		planarTextureCoordinate.push_back(GeneratePlanarUV(position, minVertex, maxVertex));
		cylindricalTextureCoordinate.push_back(GenerateCylindricalUV(position, minVertex, maxVertex));
		sphericalTextureCoordinate.push_back(GenerateSphericalUV(position));
		cubeTextureCoordinate.push_back(GenerateCubeUV(position));
	}

	for(auto vertexNormal : vertexNormals)
	{
		planarTextureCoordinateWithVertexNormal.push_back(GeneratePlanarUV(vertexNormal, minVertexNormal, maxVertexNormal));
		cylindricalTextureCoordinateWithVertexNormal.push_back(GenerateCylindricalUV(vertexNormal, minVertexNormal, maxVertexNormal));
		sphericalTextureCoordinateWithVertexNormal.push_back(GenerateSphericalUV(vertexNormal));
		cubeTextureCoordinateWithVertexNormal.push_back(GenerateCubeUV(vertexNormal));
	}
}

void Mesh::CreateMinMax()
{
	constexpr float minValue = std::numeric_limits<float>::max();
	constexpr float maxValue = std::numeric_limits<float>::min();
	minVertex = glm::vec3(minValue, minValue, minValue);
	maxVertex = glm::vec3(maxValue, maxValue, maxValue);
	for (auto position : positions)
	{
		minVertex = GetMin(minVertex, position);
		maxVertex = GetMax(maxVertex, position);
	}

	minVertexNormal = glm::vec3(minValue, minValue, minValue);
	maxVertexNormal = glm::vec3(maxValue, maxValue, maxValue);
	for (auto vertexNormal : vertexNormals)
	{
		minVertexNormal = GetMin(minVertexNormal, vertexNormal);
		maxVertexNormal = GetMax(maxVertexNormal, vertexNormal);
	}
}
