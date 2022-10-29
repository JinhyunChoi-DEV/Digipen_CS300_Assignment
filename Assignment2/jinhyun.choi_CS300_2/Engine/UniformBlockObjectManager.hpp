#pragma once
#include <glfw/glfw3.h>

#include "Camera.hpp"
#include "Light.hpp"
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
	GLint Index = 0;
};

// Light.glsl
struct UniformLightBlock
{
	const char* Names[10] = { "Light.position", "Light.direction",
		"Light.cutOff", "Light.outerCutOff",
		"Light.ambient", "Light.diffuse", "Light.specular",
		"Light.constant", "Light.linear", "Light.quadratic"
	};
	const int Vector3Count = 5;
	const int FloatCount = 5;
	int TotalCount = Vector3Count + FloatCount;
	GLuint Indices[10];
	GLint Offset[10];
	GLint Index = 1;
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
	void InitializeTransform();
	void InitializeLight();
	void BindTransformData(const Transform* model, const Camera* camera);
	void BindLightData(const Transform* model, const Light* light);

private:
	std::unordered_map<UniformBufferType, UniformBufferData> uniformBuffers;

	UniformTransformBlock transformBlock;
	UniformLightBlock lightBlock;
};