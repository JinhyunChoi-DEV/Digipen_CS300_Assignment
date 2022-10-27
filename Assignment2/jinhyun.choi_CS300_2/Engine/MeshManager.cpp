/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: MeshManager.cpp
Purpose: Making mesh manager for controlling all loaded mesh
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include "MeshManager.hpp"

MeshManager* MESHES;

MeshManager::MeshManager()
{
	MESHES = this;
}

void MeshManager::Initialize()
{}

void MeshManager::Add(std::string name, Mesh* mesh)
{
	if(meshes.count(name) > 0)
		return;

	meshes.insert(std::make_pair(name, mesh));
}

Mesh* MeshManager::GetMesh(std::string name)
{
	if (meshes.count(name) <= 0)
		return nullptr;

	return meshes[name];
}