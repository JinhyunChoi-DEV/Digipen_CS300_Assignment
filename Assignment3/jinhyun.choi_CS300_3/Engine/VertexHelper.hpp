/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: VertexHelper.hpp
Purpose: Header of helper function to make vertex data
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once
#include <vector>
#include <glm/vec3.hpp>

inline std::vector<glm::vec3> TranslateToOrigin(glm::vec3 sumAllVertex, std::vector<glm::vec3> vertices, glm::vec3& min, glm::vec3& max)
{
	std::vector<glm::vec3> result;
	const glm::vec3 origin(0, 0, 0);

	sumAllVertex /= vertices.size();
	const glm::vec3 moveToOrigin = origin - sumAllVertex;
	min += moveToOrigin;
	max += moveToOrigin;

	result.reserve(vertices.size());
	for (const auto& vertex : vertices)
		result.push_back(vertex + moveToOrigin);

	return result;
}

inline std::vector<glm::vec3> ScaleToRange(glm::vec3& min, glm::vec3& max, std::vector<glm::vec3> vertices)
{
	std::vector<glm::vec3> result;
	result.reserve(vertices.size());

	float baseFactor = std::max({ abs(min.x), abs(max.x), abs(min.y), abs(max.y), abs(min.z), abs(max.z) });
	float factor = 1.0f / baseFactor;

	for (const auto& vertex : vertices)
	{
		float x = vertex.x * factor;
		float y = vertex.y * factor;
		float z = vertex.z * factor;
		result.emplace_back(x, y, z);
	}
	min = min * factor;
	max = max * factor;

	return result;
}

inline glm::vec3 GetMax(glm::vec3 a, glm::vec3 b)
{
	float x = std::max(a.x, b.x);
	float y = std::max(a.y, b.y);
	float z = std::max(a.z, b.z);
	return { x, y, z };
}

inline glm::vec3 GetMin(glm::vec3 a, glm::vec3 b)
{
	float x = std::min(a.x, b.x);
	float y = std::min(a.y, b.y);
	float z = std::min(a.z, b.z);
	return { x, y, z };
}
