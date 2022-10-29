#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "UniformBlockObjectManager.hpp"
#include "ObjectManager.hpp"

class Mesh;

void UniformBlockObjectManager::InitializeTransform()
{
	UniformBufferData data;
	data.index = transformBlock.Index;

	auto count = transformBlock.Count;

	glGenBuffers(1, &data.UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, data.UBO);
	glBufferData(GL_UNIFORM_BUFFER, count * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, data.index, data.UBO, 0, count * sizeof(glm::mat4));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	uniformBuffers.insert(std::make_pair(UniformBufferType::Transform, data));
}

void UniformBlockObjectManager::InitializeLight()
{
	UniformBufferData data;
	data.index = lightBlock.Index;

	auto vec3Count = lightBlock.Vector3Count;
	auto floatCount = lightBlock.FloatCount;
	int size = sizeof(glm::vec3) * vec3Count + sizeof(GLfloat) * floatCount;

	glGenBuffers(1, &data.UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, data.UBO);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, data.index, data.UBO, 0, size);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	uniformBuffers.insert(std::make_pair(UniformBufferType::Light, data));
}

void UniformBlockObjectManager::BindTransformData(const Transform* model, const Camera* camera)
{
	if (uniformBuffers.find(UniformBufferType::Transform) == uniformBuffers.end())
		InitializeTransform();

	auto bufferInfo = uniformBuffers.find(UniformBufferType::Transform);
	auto data = bufferInfo->second;

	glBindBuffer(GL_UNIFORM_BUFFER, data.UBO);

	GLint offset = 0;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(model->GetMatrix()));
	offset += sizeof(glm::mat4);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(camera->GetView()));
	offset += sizeof(glm::mat4);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(camera->GetProjection()));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void UniformBlockObjectManager::BindLightData(const Transform* model, const Light* light)
{
	if (uniformBuffers.find(UniformBufferType::Light) == uniformBuffers.end())
		InitializeLight();

	auto bufferInfo = uniformBuffers.find(UniformBufferType::Light);
	auto data = bufferInfo->second;
	float cutOff = light->GetCutOffAngle();
	float outerCutOff = light->GetOuterCutOffAngle();
	float constant = light->GetConstant();
	float linear = light->GetLinear();
	float quadratic = light->GetQuadratic();

	int baseSize = 16;
	glBindBuffer(GL_UNIFORM_BUFFER, data.UBO);

	GLint offset = 0;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(model->GetPosition()));
	offset += baseSize;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(light->GetDirection()));
	offset += baseSize;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &cutOff);
	offset += baseSize;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &outerCutOff);
	offset += baseSize;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(light->GetAmientIntensity()));
	offset += baseSize;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(light->GetDiffuseIntensity()));
	offset += baseSize;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(light->GetSpecularIntensity()));
	offset += baseSize;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &constant);
	offset += baseSize;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &linear);
	offset += baseSize;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &quadratic);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
