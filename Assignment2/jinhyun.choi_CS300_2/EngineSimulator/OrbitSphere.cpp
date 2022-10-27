/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: OrbitSphere.cpp
Purpose: Making procedurally orbit sphere (lines)
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include <glm/ext/scalar_constants.hpp>

#include "OrbitSphere.hpp"
#include "VertexHelper.hpp"

OrbitSphere::OrbitSphere(std::string name, float radius, int sector)
{
	std::vector<glm::vec3> positions;

	glm::vec3 min(0, 0, 0);
	glm::vec3 max(0, 0, 0);
	glm::vec3 sumAllVertex(0, 0, 0);
	glm::vec3 currentVertex(0, 0, 0);

	constexpr float pi = glm::pi<float>();
	float step = 2 * pi / sector;
	for(int i =0; i <= sector; ++i)
	{
		if (i > 1)
			positions.push_back(currentVertex);

		float angle = step * i;
		currentVertex.x = radius * cosf(angle);
		currentVertex.y = 0.0f;
		currentVertex.z = radius * sinf(angle);

		sumAllVertex.x += currentVertex.x;
		sumAllVertex.y += currentVertex.y;
		sumAllVertex.z += currentVertex.z;

		min = GetMin(currentVertex, min);
		max = GetMax(currentVertex, max);

		positions.push_back(currentVertex);
	}

	positions = TranslateToOrigin(sumAllVertex, positions, min, max);
	positions = ScaleToRange(min, max, positions);

	mesh = new Mesh(name, positions, Line);
}
