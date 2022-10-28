#include <glad/glad.h>
#include <iostream>
#include "UniformBlockObjectManager.hpp"

void UniformBlockObjectManager::Set(const Shader* shader, const Transform* model, const Camera* camera)
{
	if (uniformBuffers.find(UniformBufferType::Transform) == uniformBuffers.end())
		CreateTransformUniformBlock(shader, model, camera);
	else
		BindTransformData(shader, model, camera);

}

void UniformBlockObjectManager::CreateTransformUniformBlock(const Shader* shader, const Transform* model, const Camera* camera)
{
	unsigned int programId = shader->ProgramID;

	UniformBufferData data;
	data.index = glGetUniformBlockIndex(programId, "Transform");
	glGetActiveUniformBlockiv(programId, data.index, GL_UNIFORM_BLOCK_DATA_SIZE, &data.size);

	auto count = transformBlock.Count;
	auto names = transformBlock.Names;
	auto indices = transformBlock.Indices;
	auto offset = transformBlock.Offset;

	glGetUniformIndices(programId, count, names, indices);
	glGetActiveUniformsiv(programId, count, indices, GL_UNIFORM_OFFSET, offset);

	std::vector<glm::mat4> buffersData = { model->GetMatrix(), camera->GetView(), camera->GetProjection() };
	data.buffer = (GLubyte*)malloc(data.size);

	for(int i =0; i<count; ++i)
	{
		auto matrix = buffersData[i];
		memcpy(data.buffer + offset[i], &matrix[0], sizeof(glm::mat4));
	}

	glGenBuffers(1, &data.UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, data.UBO);
	glBufferData(GL_UNIFORM_BUFFER, data.size, data.buffer, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, data.index, data.UBO);

	uniformBuffers.insert(std::make_pair(UniformBufferType::Transform, data));
}

void UniformBlockObjectManager::BindTransformData(const Shader* shader, const Transform* model, const Camera* camera)
{
	auto buffers = uniformBuffers.find(UniformBufferType::Transform);
	if (buffers == uniformBuffers.end())
	{
		CreateTransformUniformBlock(shader, model, camera);
		return;
	}

	auto count = transformBlock.Count;
	auto offset = transformBlock.Offset;
	std::vector<glm::mat4> buffersData = { model->GetMatrix(), camera->GetView(), camera->GetProjection() };
	auto data = buffers->second;

	for (int i = 0; i < count; ++i)
	{
		auto matrix = buffersData[i];
		memcpy(data.buffer + offset[i], &matrix[0], sizeof(glm::mat4));
	}

	glBindBuffer(GL_UNIFORM_BUFFER, data.UBO);
	glBufferData(GL_UNIFORM_BUFFER, data.size, data.buffer, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, data.index, data.UBO);
}


void UniformBlockObjectManager::CreateLightUniformBlock(unsigned int programId)
{
	if (uniformBuffers.find(UniformBufferType::Light) == uniformBuffers.end())
		return;
}
