/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.cpp
Purpose: Making a mesh for making object with holding model information like vertices, normals
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include "Mesh.hpp"
#include "Graphic.hpp"
#include "MeshManager.hpp"
#include "Shader.hpp"
#include "UVGenerator.hpp"

Mesh::Mesh(std::string name, 
	std::vector<glm::vec3> positions, std::vector<glm::vec3> vertexNormal, std::vector<glm::vec3> faceNormal, std::vector<unsigned int> indices,
	glm::vec3 minVertex, glm::vec3 maxVertex, DrawType type)
	: name(name), positions(positions), vertexNormals(vertexNormal), faceNormal(faceNormal),
	indices(indices), minVertex(minVertex), maxVertex(maxVertex), objectColor(glm::vec3(0.8, 0.8, 0.8)), type(type)
{
	shader = GRAPHIC->GetShader("PhongShading");

	if(type == DrawType::ObjectModel)
	{
		CreateVertexNormalLines();
		CreateFaceNormalLines();
		CreateTextureCoordinates();
	}
}

Mesh::Mesh(std::string name, std::vector<glm::vec3> vertices, DrawType type)
	: name(name), positions(vertices), objectColor(1, 1, 1), type(type)
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
	indices = mesh->GetIndices();
	objectColor = mesh->GetColor();
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

void Mesh::SetColor(glm::vec3 color)
{
	objectColor = color;
}

void Mesh::SetDrawType(DrawType type)
{
	this->type = type;
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
		planarTextureCoordinateWithVertexNormal.push_back(GeneratePlanarUV(vertexNormal, minVertex, maxVertex));
		cylindricalTextureCoordinateWithVertexNormal.push_back(GenerateCylindricalUV(vertexNormal, minVertex, maxVertex));
		sphericalTextureCoordinateWithVertexNormal.push_back(GenerateSphericalUV(vertexNormal));
		cubeTextureCoordinateWithVertexNormal.push_back(GenerateCubeUV(vertexNormal));
	}
}
