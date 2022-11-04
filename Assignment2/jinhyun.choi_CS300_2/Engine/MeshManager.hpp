/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: MeshManager.hpp
Purpose: Header to MeshManager class
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once
#include <unordered_map>

class Mesh;

class MeshManager
{
public:
	MeshManager();
	void Initialize();

	void Add(std::string name, Mesh* mesh);
	Mesh* GetMesh(std::string name);

private:
	std::unordered_map<std::string, Mesh*> meshes;
};

extern MeshManager* MESHES;
