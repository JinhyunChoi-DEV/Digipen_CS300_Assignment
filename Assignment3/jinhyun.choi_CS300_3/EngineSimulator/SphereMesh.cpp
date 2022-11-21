/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: SphereMesh.cpp
Purpose: Making procedurally sphere mesh
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include <unordered_map>
#include <glm/ext/scalar_constants.hpp>

#include "SphereMesh.hpp"
#include "VertexHelper.hpp"

SphereMesh::SphereMesh(std::string name, float radius, int stack, int sector)
	: radius(radius)
{
	constexpr auto pi = glm::pi<float>();
	float sectorStep = 2 * pi / (float)sector;
	float stackStep = pi / (float)stack;
	float sectorAngle, stackAngle = 0.0f;
	float lengthInv = 1.0f / radius;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> vertexNormal;
	std::vector<glm::vec3> faceNormal;
	std::vector<unsigned int> indices;

	glm::vec3 min(0, 0, 0);
	glm::vec3 max(0, 0, 0);
	glm::vec3 sumAllVertex(0, 0, 0);

	// Create position and position Normal
	for (int i = 0; i <= stack; ++i)
	{
		stackAngle = pi / 2 - (float)i * stackStep;
		float xy = radius * cosf(stackAngle);
		float z = radius * sinf(stackAngle);

		for (int j = 0; j <= sector; ++j)
		{
			sectorAngle = (float)j * sectorStep;
			float x = xy * cosf(sectorAngle);
			float y = xy * sinf(sectorAngle);
			glm::vec3 currentVertex(x, y, z);

			sumAllVertex.x += currentVertex.x;
			sumAllVertex.y += currentVertex.y;
			sumAllVertex.z += currentVertex.z;

			min = GetMin(currentVertex, min);
			max = GetMax(currentVertex, max);

			positions.push_back(currentVertex);
		}
	}

	// Index
	for (int i = 0; i < stack; ++i)
	{
		int k1 = i * (sector + 1);
		int k2 = k1 + sector + 1;

		for (int j = 0; j < sector; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (stack - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}
	positions = TranslateToOrigin(sumAllVertex, positions, min, max);
	positions = ScaleToRange(min, max, positions);

	for (const auto& vertex : positions)
	{
		float nx = vertex.x * lengthInv;
		float ny = vertex.y * lengthInv;
		float nz = vertex.z * lengthInv;
		vertexNormal.emplace_back(nx, ny, nz);
	}

	std::unordered_map<int, std::vector<glm::vec3>> vnCollections;
	for (int i = 0; i < (int)indices.size(); i += 3)
	{
		unsigned firstFactor = (unsigned)i;
		unsigned secondFactor = (unsigned)i+1;
		unsigned thirdFactor = (unsigned)i+2;

		int firstIndex = (int)indices[firstFactor];
		int secondIndex = (int)indices[secondFactor];
		int thirdIndex= (int)indices[thirdFactor];

		auto first = positions[firstIndex];
		auto second = positions[secondIndex];
		auto third = positions[thirdIndex];

		auto v1 = second - first;
		auto v2 = third - first;
		auto normal = normalize(cross(v1, v2));
		faceNormal.push_back(normal);

		vnCollections[firstIndex].push_back(normal);
		vnCollections[secondIndex].push_back(normal);
		vnCollections[thirdIndex].push_back(normal);
	}

	mesh = new Mesh(name, positions, vertexNormal, faceNormal, indices, DrawType::Solid);
	mesh->SetShader("Solid");
}
