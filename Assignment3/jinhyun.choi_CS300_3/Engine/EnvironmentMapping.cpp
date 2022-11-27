/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: EnvironmentMapping.cpp
Purpose: the cpp file for EnvironmentMapping data and functions
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/27/2022
End Header --------------------------------------------------------*/

#include <iostream>

#include "EnvironmentMapping.hpp"
#include "MeshManager.hpp"
#include "ObjectLoader.hpp"
#include "TextureManager.hpp"
#include "Shader.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include <glad/glad.h>

EnvironmentMapping::EnvironmentMapping(glm::vec2 size)
{
	drawBuffers = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT };
	OBJECT_LOADER->Load("quad.obj", "frameBufferDraw");
	mesh = MESHES->GetMesh("frameBufferDraw");
	mesh->SetShader("FrameBuffer");

	width = (int)size.x;
	height = (int)size.y;

	baseTransform = new Transform();
	baseTransform->SetTranslate({ 0,0,0 });
	cam = new Camera();
	cam->SetFOV(90.0f);

	visualizeType = VisualizeType::BothEnvironment;

	refractiveIndex = 1.0f;
	fresnelPower = 5.0f;
	colorMixRatio = 0.9f;

	CreateFrameBuffer();
}

void EnvironmentMapping::CheckRenterToTexture()
{
	if (textureWidth != width || textureHeight != height)
	{
		CreateFrameBuffer();
	}
}

void EnvironmentMapping::Bind(int index)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderToTexture[index], 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EnvironmentMapping::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void EnvironmentMapping::Update(Shader* shader)
{
	shader->Set("type", (int)visualizeType);
	shader->Set("refractiveIndex", refractiveIndex);
	shader->Set("fresnelPower", fresnelPower);
	shader->Set("colorMixRatio", colorMixRatio);

	for (int i = 0; i < 6; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, renderToTexture[i]);
		shader->Set(samplerName[i], i);
	}
}

void EnvironmentMapping::CreateFrameBuffer()
{
	if (initialize)
	{
		glDeleteFramebuffers(1, &frameBufferObject);
		glDeleteTextures(6, renderToTexture.data());
		renderToTexture.clear();
		glDeleteRenderbuffers(1, &depthRenderBuffer);
	}

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
	}

	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

	glDrawBuffers(2, drawBuffers.data());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	textureWidth = width;
	textureHeight = height;
}
