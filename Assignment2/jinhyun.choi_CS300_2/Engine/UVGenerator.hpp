#pragma once
#include <glm/glm.hpp>

glm::vec2 GeneratePlanarUV(glm::vec3 vertex, glm::vec3 max, glm::vec3 min)
{
	float u = (vertex.x - min.x) / (max.x - min.x);
	float v = (vertex.y - min.y) / (max.y - min.y);
	return glm::vec2(u, v);
}

glm::vec2 GenerateCylindricalUV(glm::vec3 vertex, glm::vec3 max, glm::vec3 min)
{
	float x = vertex.x;
	float y = vertex.y;
	float z = vertex.z;

	// atan2 -> return -pi to pi
	float theta = glm::degrees(atan2(y, x)) + 180.f;

	float u = theta / 360.0f;
	float v = (z - min.z) / (max.z - min.z);
	return glm::vec2(u, v);
}

glm::vec2 GenerateSphericalUV(glm::vec3 vertex)
{
	float x = vertex.x;
	float y = vertex.y;
	float z = vertex.z;

	float r = sqrt(x * x + y * y + z * z);
	// atan2 -> return -pi to pi
	float theta = glm::degrees(atan2(y , x)) + 180.f;
	float phi = acos(z / r);

	float u = theta / 360.0f;
	float v = phi / 180.0f;
	return glm::vec2(u, v);
}

glm::vec2 GenerateCubeUV(glm::vec3 vertex)
{
	glm::vec3 absVector = abs(vertex);
	float u = 0.0f;
	float v = 0.0f;

	// x coordinate
	if (absVector.x >= absVector.y && absVector.x >= absVector.z)
	{
		if (vertex.x < 0.0f)
			u = vertex.z;
		else
			u = -vertex.z;

		v = vertex.y;
	}

	// y coordinate
	if (absVector.y >= absVector.x && absVector.y >= absVector.z)
	{
		if (vertex.y < 0.0f)
			v = vertex.z;
		else
			v = -vertex.z;

		u = vertex.x;
	}

	// z coordinate 
	if(absVector.z >= absVector.x && absVector.z >= absVector.y)
	{
		if (vertex.z < 0.0f)
			u = -vertex.x;
		else
			u = vertex.x;

		v = vertex.y;
	}

	glm::vec2 uv{ u,v };
	return (uv + glm::vec2(1.0f)) * 0.5f;
}