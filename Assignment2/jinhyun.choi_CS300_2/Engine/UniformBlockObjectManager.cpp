#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "UniformBlockObjectManager.hpp"

#include "Graphic.hpp"
#include "ObjectManager.hpp"

class Mesh;

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
	data.size = 112;

	glGenBuffers(1, &data.UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, data.UBO);
	glBufferData(GL_UNIFORM_BUFFER, data.size, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, data.index, data.UBO, 0, data.size);
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
	unsigned int type = static_cast<std::underlying_type<LightType>::type>(light->GetType());
	float cutOff = light->GetCutOffAngle();
	float outerCutOff = light->GetOuterCutOffAngle();
	float constant = light->GetConstant();
	float linear = light->GetLinear();
	float quadratic = light->GetQuadratic();

	glBindBuffer(GL_UNIFORM_BUFFER, data.UBO);

	GLint offset = 0;
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
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &cutOff);
	offset += 4;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &outerCutOff);
	offset += 4;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &constant);
	offset += 4;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &linear);
	offset += 4;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &quadratic);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
