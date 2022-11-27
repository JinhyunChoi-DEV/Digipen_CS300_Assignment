/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ObjectLoader.hpp
Purpose: Header of object loader class
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once

#include <glm/vec3.hpp>

class ObjectLoader
{
public:
	ObjectLoader();
	bool Load(const char* fileName, std::string name);

	const char* GetObjFileDir() const { return objFilePath; }

private:
	const char* objFilePath = "../Objects/";
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> vertexNormal;
	std::vector<glm::vec3> faceNormal;
	std::vector<glm::ivec3> faces;
	std::vector<unsigned int> indices;
	bool isMultipleIndex = false;
};

extern ObjectLoader* OBJECT_LOADER;