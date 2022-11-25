#include <iostream>

#include "EnvironmentMapping.hpp"

#include "Graphic.hpp"
#include "MeshManager.hpp"
#include "ObjectLoader.hpp"
#include "Shader.hpp"
#include "TextureManager.hpp"
#include "UniformBlockObjectManager.hpp"

EnvironmentMapping::EnvironmentMapping(glm::vec2 size)
{
	OBJECT_LOADER->Load("quad.obj", "frameBufferDraw");
	mesh = MESHES->GetMesh("frameBufferDraw");
	mesh->SetShader("FrameBuffer");

	width = (int)size.x;
	height = (int)size.y;

	baseTransform = new Transform();
	baseTransform->SetTranslate({ 0,0,0 });
	cam = new Camera();
	cam->SetFOV(90.0f);

	CreateFrameBuffer();
}

void EnvironmentMapping::Bind(int index)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
	glDrawBuffers(2, drawBuffers.data());

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderToTexture[index], 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EnvironmentMapping::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void EnvironmentMapping::UpdateMappingTexture(Shader* shader)
{
	for (int i = 0; i < 6; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, renderToTexture[i]);
		shader->Set(samplerName[i], i);
	}
}

void EnvironmentMapping::CreateFrameBuffer()
{
	auto temp = TEXTURES->Get("top");

	glGenFramebuffers(1, &frameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

	renderToTexture.resize(6);
	glGenTextures(6, renderToTexture.data());

	for (int i = 0; i < 6; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, renderToTexture[i]);
		GLint type = 0;
		if (temp.GetChannel() == 3)
			type = GL_RGB;
		else
			type = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderToTexture[i], 0);
	}

	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

	glDrawBuffers(2, drawBuffers.data());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
