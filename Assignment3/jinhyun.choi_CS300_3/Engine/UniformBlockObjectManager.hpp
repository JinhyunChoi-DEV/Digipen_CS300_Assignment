/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: UniformBlockObjectManager.hpp
Purpose: the header file of UniformBlockObjectManager for UniformBuffer data and functions
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

#pragma once
#include <glfw/glfw3.h>
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Transform.hpp"

enum class UniformBufferType
{
	Transform, Light
};

struct UniformBufferData
{
	GLuint UBO;
	GLuint index;
	GLint size;
};

class Object;
class UniformBlockObjectManager
{
public:
	UniformBlockObjectManager();

	void InitializeTransform();
	void InitializeLight();

	void BindTransformData(const Transform* model, const glm::mat4 view, const glm::mat4 projection);
	void BindLightData(std::vector<Object* > objects, glm::vec3 attenuationConstants, glm::vec3 globalAmbient, glm::vec3 fog, float fogMin, float fogMax);
private:
	std::unordered_map<UniformBufferType, UniformBufferData> uniformBuffers;

	unsigned int transformIndex = 0;
	unsigned int lightIndex = 1;
	unsigned int lightMaxCount = 16;
};

extern UniformBlockObjectManager* UNIFORMBLOCK_OBJECTS;