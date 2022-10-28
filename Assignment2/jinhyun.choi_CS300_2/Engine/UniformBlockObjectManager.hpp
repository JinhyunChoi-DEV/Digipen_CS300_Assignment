#pragma once
#include <glfw/glfw3.h>

#include "Camera.hpp"
#include "Shader.hpp"
#include "Transform.hpp"

enum class UniformBufferType
{
	Transform, Light
};


struct UniformTransformBlock
{
	const char* Names[3] = {"Transform.model", "Transform.view", "Transform.projection"};
	const int Count = 3;
	GLuint Indices[3];
	GLint Offset[3];
};

struct UniformLightBlock
{
};

struct UniformBufferData
{
	GLuint UBO;
	GLuint index;
	GLint size;
	GLubyte* buffer;
};

class UniformBlockObjectManager
{
public:
	void Set(const Shader* shader, const Transform* model, const Camera* camera);

private:
	void CreateTransformUniformBlock(const Shader* shader, const Transform* model, const Camera* camera);
	void BindTransformData(const Shader* shader, const Transform* model, const Camera* camera);
	void CreateLightUniformBlock(unsigned int programId);
	std::unordered_map<UniformBufferType, UniformBufferData> uniformBuffers;

	UniformTransformBlock transformBlock;
	UniformLightBlock lightBlock;
};