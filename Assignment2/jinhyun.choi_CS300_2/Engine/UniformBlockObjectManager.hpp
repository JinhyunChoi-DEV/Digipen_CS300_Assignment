#pragma once
#include <glfw/glfw3.h>

#include "Camera.hpp"
#include "Shader.hpp"
#include "Transform.hpp"

enum class UniformBufferType
{
	Transform, Light
};

// TransformModel.glsl
struct UniformTransformBlock
{
	const char* Names[3] = {"Transform.model", "Transform.view", "Transform.projection"};
	const int Count = 3;
	GLuint Indices[3];
	GLint Offset[3];
};

// Light.glsl
struct UniformLightBlock
{
	const char* Names[10] = { "Light.position", "Light.direction",
		"Light.cutOff", "Light.outerCutOff",
		"Light.ambient", "Light.diffuse", "Light.specular",
		"Light.constant", "Light.linear", "Light.quadratic"
	};
	const int Count = 10;
	GLuint Indices[10];
	GLint Offset[10];
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
	void Set2(const Shader* shader, const Transform* model, const Camera* camera);

private:
	void CreateTransformUniformBlock(const Shader* shader, const Transform* model, const Camera* camera);
	void BindTransformData(const Shader* shader, const Transform* model, const Camera* camera);
	void CreateLightUniformBlock(const Shader* shader, const Transform* model, const Camera* camera);
	void BindLightData(const Shader* shader, const Transform* model, const Camera* camera);
	std::unordered_map<UniformBufferType, UniformBufferData> uniformBuffers;

	UniformTransformBlock transformBlock;
	UniformLightBlock lightBlock;
};