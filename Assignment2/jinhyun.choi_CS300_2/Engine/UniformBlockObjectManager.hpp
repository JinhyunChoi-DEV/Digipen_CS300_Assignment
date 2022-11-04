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
	void InitializeTransform();
	void InitializeLight();

	void BindTransformData(const Transform* model, const Camera* camera);
	void BindLightData(std::vector<Object* > objects, glm::vec3 attenuationConstants, glm::vec3 globalAmbient, glm::vec3 fog, float fogMin, float fogMax);
private:
	std::unordered_map<UniformBufferType, UniformBufferData> uniformBuffers;

	unsigned int transformIndex = 0;
	unsigned int lightIndex = 1;
	unsigned int lightMaxCount = 16;
};