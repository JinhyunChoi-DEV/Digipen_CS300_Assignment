/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ObjectLoader.cpp
Purpose: Making Object doing to read obj file and load it to object
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/vec3.hpp>
#include <algorithm>
#include <numeric>

#include "ObjectManager.hpp"
#include "ObjectLoader.hpp"
#include "Object.hpp"
#include "VertexHelper.hpp"
#include "Mesh.hpp"
#include "MeshManager.hpp"

namespace 
{
	void CreateObject(std::string name, std::vector<glm::vec3> positions, std::vector<glm::vec3> vertexNormal,
		std::vector<glm::vec3> faceNormal, std::vector<unsigned int> indices);
}


bool ObjectLoader::Load(const char* fileName, std::string name)
{
	auto path = std::string(objFilePath) + std::string(fileName);
	std::ifstream file(path);
	std::stringstream fileStream;

	isMultipleIndex = false;
	positions.clear();
	vertexNormal.clear();
	faceNormal.clear();
	faces.clear();
	indices.clear();

	if (!file.is_open())
	{
		std::cout << "Error-ObjectLoad: Fail To Open Obj File: " << path << std::endl;
		return false;
	}

	std::string line;
	std::string prefix;
	float x, y, z = 0;
	unsigned int firstVertex, secondVertex, thirdVertex;

	glm::vec3 min(0, 0, 0);
	glm::vec3 max(0, 0, 0);
	glm::vec3 sumAllVertex(0, 0, 0);

	while (std::getline(file, line))
	{
		//should call clear for reset previous EOF data.
		fileStream.clear();
		fileStream.str(line);
		if (line.size() > 0)
			fileStream >> prefix;
		else
			prefix = "";

		if (prefix == "v") // Vertex position
		{
			fileStream >> x >> y >> z;
			glm::vec3 currentVertex(x, y, z);

			sumAllVertex.x += currentVertex.x;
			sumAllVertex.y += currentVertex.y;
			sumAllVertex.z += currentVertex.z;

			min = GetMin(currentVertex, min);
			max = GetMax(currentVertex, max);

			positions.emplace_back(currentVertex);
		}
		if (prefix == "f") // Face
		{
			fileStream >> firstVertex >> secondVertex >> thirdVertex;
			// need to minus one because face of index start to one but position start to zero.
			faces.emplace_back(firstVertex - 1, secondVertex - 1, thirdVertex - 1);
			indices.insert(indices.end(), { firstVertex - 1, secondVertex - 1, thirdVertex - 1 });

			int remainVertex = 0;
			while (fileStream >> remainVertex)
			{
				if (!isMultipleIndex)
					isMultipleIndex = true;

				secondVertex = thirdVertex;
				thirdVertex = remainVertex;
				faces.emplace_back(firstVertex - 1, secondVertex - 1, thirdVertex - 1);
				indices.insert(indices.end(), { firstVertex - 1, secondVertex - 1, thirdVertex - 1 });
			}
		}
	}
	positions = TranslateToOrigin(sumAllVertex, positions, min, max);
	positions = ScaleToRange(min, max, positions);
	vertexNormal.resize(positions.size(), glm::vec3{ 0,0,0 });

	// Make Face and Vertex Normal;
	std::unordered_map<int, std::vector<glm::vec3>> vnCollections;
	for (auto face : faces)
	{
		auto first = positions[face.x];
		auto second = positions[face.y];
		auto third = positions[face.z];

		auto v1 = second - first;
		auto v2 = third - first;
		auto normal = normalize(cross(v1, v2));
		faceNormal.push_back(normal);

		vnCollections[face.x].push_back(normal);
		vnCollections[face.y].push_back(normal);
		vnCollections[face.z].push_back(normal);
	}

	for (auto vn : vnCollections)
	{
		auto index = vn.first;
		auto normals = vn.second;

		auto isAlmostSame = [](glm::vec3 v1, glm::vec3 v2) {return length(v1 - v2) <= (std::numeric_limits<float>::epsilon() * 10.0f); };
		normals.erase(std::unique(normals.begin(), normals.end(), isAlmostSame), normals.end());

		auto sumNormals = std::accumulate(normals.begin(), normals.end(), glm::vec3{ 0,0,0 });
		vertexNormal[index] = normalize(sumNormals);
	}

	Mesh* mesh = new Mesh(name, positions, vertexNormal, faceNormal, indices, ObjectModel);
	mesh->SetMultipleFaceIndex(isMultipleIndex);
	MESHES->Add(name, mesh);

	return true;
}

bool ObjectLoader::LoadAndCreate(const char* fileName, std::string name)
{
	Load(fileName, name);
	CreateObject(name, positions, vertexNormal, faceNormal, indices);

	return true;
}

namespace 
{
	void CreateObject(std::string name, std::vector<glm::vec3> positions, std::vector<glm::vec3> vertexNormal,
		std::vector<glm::vec3> faceNormal, std::vector<unsigned int> indices)
	{
		Mesh* mesh = new Mesh(name, positions, vertexNormal, faceNormal, indices, ObjectModel);

		Object* object = new Object();
		object->AddComponent(mesh);
		OBJECTMANAGER->Add(name, object);
		MESHES->Add(name, mesh);
	}
}
