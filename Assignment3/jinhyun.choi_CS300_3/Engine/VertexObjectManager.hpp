/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: VertexObjectManager.hpp
Purpose: Header of VertexObjectManager class
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

#pragma once
#include <unordered_map>
#include "Mesh.hpp"

class VertexObjectManager
{
public:
	VertexObjectManager();

	unsigned int GetObjectVAO(const Mesh* mesh);
	unsigned int GetVertexNormalVAO(const Mesh* mesh);
	unsigned int GetFaceNormalVAO(const Mesh* mesh);

	void RebindTexture(const Mesh* mesh, const Texture* texture);

private:
	void GenerateObject(std::string name, const Mesh* mesh);
	void GenerateLines(std::string name, const Mesh* mesh);
	void GenerateNormalLines(std::string name, const Mesh* mesh, bool isVertexNormal);

	std::unordered_map<std::string, unsigned int> vertexArrays;
	std::unordered_map<std::string, unsigned int> vertexBuffers;
	std::unordered_map<std::string, unsigned int> elementBuffers;
};
extern VertexObjectManager* VERTEX_OBJECTS;
