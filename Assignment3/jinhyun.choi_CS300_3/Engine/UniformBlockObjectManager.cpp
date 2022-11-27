/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: UniformBlockObjectManager.cpp
Purpose: the cpp file of UniformBlockObjectManager for UniformBuffer data and functions
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "UniformBlockObjectManager.hpp"
#include "Light.hpp"
#include "Object.hpp"
#include "ObjectManager.hpp"

class Mesh;

UniformBlockObjectManager* UNIFORMBLOCK_OBJECTS = nullptr;
UniformBlockObjectManager::UniformBlockObjectManager()
{
	if (UNIFORMBLOCK_OBJECTS == nullptr)
		UNIFORMBLOCK_OBJECTS = this;
}

void UniformBlockObjectManager::InitializeTransform()
{
	UniformBufferData data;
	data.index = transformIndex;
	data.size = 3 * sizeof(glm::mat4);

	glGenBuffers(1, &data.UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, data.UBO);
	glBufferData(GL_UNIFORM_BUFFER, data.size, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, data.index, data.UBO, 0, data.size);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	uniformBuffers.insert(std::make_pair(UniformBufferType::Transform, data));
}

void UniformBlockObjectManager::InitializeLight()
{
	UniformBufferData data;
	data.index = lightIndex;
	// Light.glsl
	data.size = 112 * lightMaxCount + 64;

	glGenBuffers(1, &data.UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, data.UBO);
	glBufferData(GL_UNIFORM_BUFFER, data.size, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, data.index, data.UBO, 0, data.size);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	uniformBuffers.insert(std::make_pair(UniformBufferType::Light, data));
}

void UniformBlockObjectManager::BindTransformData(const Transform* model, const glm::mat4 view, const glm::mat4 projection)
{
	if (uniformBuffers.find(UniformBufferType::Transform) == uniformBuffers.end())
		InitializeTransform();

	auto bufferInfo = uniformBuffers.find(UniformBufferType::Transform);
	auto data = bufferInfo->second;

	glBindBuffer(GL_UNIFORM_BUFFER, data.UBO);

	GLint offset = 0;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(model->GetMatrix()));
	offset += sizeof(glm::mat4);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(view));
	offset += sizeof(glm::mat4);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(projection));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void UniformBlockObjectManager::BindLightData(std::vector<Object* > objects, glm::vec3 attenuationConstants, glm::vec3 globalAmbient, glm::vec3 fog, float fogMin, float fogMax)
{
	int activeCount = (int)objects.size();

	if (uniformBuffers.find(UniformBufferType::Light) == uniformBuffers.end())
		InitializeLight();

	auto bufferInfo = uniformBuffers.find(UniformBufferType::Light);
	auto data = bufferInfo->second;
	glBindBuffer(GL_UNIFORM_BUFFER, data.UBO);

	GLint offset = 0;
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(unsigned int), &activeCount);
	offset += 4;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &fogMin);
	offset += 4;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &fogMax);
	offset += 8;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(attenuationConstants));
	offset += 16;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(globalAmbient));
	offset += 16;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(fog));
	offset += 16;

	for (int i = 0; i < activeCount; ++i)
	{
		auto model = objects[i]->GetComponent<Transform>();
		auto light = objects[i]->GetComponent<Light>();

		unsigned int type = static_cast<std::underlying_type<LightType>::type>(light->GetType());
		float innerAngle = light->GetInnerAngle();
		float outerAngle = light->GetOuterAngle();
		float fallOut = light->GetFallOut();

		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(unsigned int), &type);
		offset += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(model->GetPosition()));
		offset += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(light->GetDirection()));
		offset += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(light->GetAmientIntensity()));
		offset += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(light->GetDiffuseIntensity()));
		offset += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(light->GetSpecularIntensity()));
		offset += 12;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &innerAngle);
		offset += 4;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &outerAngle);
		offset += 4;
		glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &fallOut);
		offset += 12;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
