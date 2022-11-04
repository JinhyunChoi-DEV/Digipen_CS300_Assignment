/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: SphereMesh.hpp
Purpose: Header of making SphereMesh class
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once
#include "Mesh.hpp"

class SphereMesh
{
public:
	SphereMesh(std::string name, float radius, int stack, int sector);

	Mesh* GetMesh() const { return mesh; }

private:
	float radius;
	Mesh* mesh;
};