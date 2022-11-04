/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: UVGenerator.hpp
Purpose: the header file for UV generating functions
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

#pragma once
#include <glm/glm.hpp>

inline glm::vec2 GeneratePlanarUV(glm::vec3 vertex, glm::vec3 min, glm::vec3 max)
{
	float u = (vertex.x - min.x) / (max.x - min.x);
	float v = (vertex.y - min.y) / (max.y - min.y);
	return glm::vec2(u, v);
}

inline glm::vec2 GenerateCylindricalUV(glm::vec3 vertex, glm::vec3 min, glm::vec3 max)
{
	float x = vertex.x;
	float y = vertex.y;
	float z = vertex.z;

	// atan2 -> return -pi to pi
	float theta = glm::degrees(atan2(z, x)) + 180.f;

	float u = theta / 360.0f;
	float v = (y - min.y) / (max.y - min.y);
	return glm::vec2(u, v);
}

inline glm::vec2 GenerateSphericalUV(glm::vec3 vertex)
{
	float x = vertex.x;
	float y = vertex.y;
	float z = vertex.z;

	float r = sqrt(x * x + y * y + z * z);
	// atan2 -> return -pi to pi
	float theta = glm::degrees(atan2(z , x)) + 180.f;
	float phi = acos(y / r);

	float u = theta / 360.0f;
	float v = 1.0f - (phi / 180.0f);
	return glm::vec2(u, v);
}

inline glm::vec2 GenerateCubeUV(glm::vec3 vertex)
{
	glm::vec3 absVector = abs(vertex);
	float u = 0.0f;
	float v = 0.0f;

	// x coordinate
	if (absVector.x >= absVector.y && absVector.x >= absVector.z)
	{
		if (vertex.x < 0.0f)
			u = vertex.z / absVector.x;
		else
			u = -vertex.z / absVector.x;

		v = vertex.y / absVector.x;
	}

	// y coordinate
	else if (absVector.y >= absVector.x && absVector.y >= absVector.z)
	{
		if (vertex.y < 0.0f)
			v = vertex.z / absVector.y;
		else
			v = -vertex.z / absVector.y;

		u = vertex.x / absVector.y;
	}

	// z coordinate 
	else if(absVector.z >= absVector.x && absVector.z >= absVector.y)
	{
		if (vertex.z < 0.0f)
			u = -vertex.x / absVector.z;
		else
			u = vertex.x / absVector.z;

		v = vertex.y / absVector.z;
	}

	glm::vec2 uv{ u,v };
	return (uv + glm::vec2(1.0f)) * 0.5f;
}