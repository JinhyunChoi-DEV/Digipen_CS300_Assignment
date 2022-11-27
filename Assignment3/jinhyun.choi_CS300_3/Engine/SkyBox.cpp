/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: SkyBox.cpp
Purpose: the cpp file for SkyBox data and functions
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/27/2022
End Header --------------------------------------------------------*/

#include <iostream>
#include <glad/glad.h>

#include "Mesh.hpp"
#include "MeshManager.hpp"
#include "Skybox.hpp"
#include "Object.hpp"
#include "ObjectLoader.hpp"
#include "ObjectManager.hpp"
#include "TextureManager.hpp"
#include "Transform.hpp"
#include "Shader.hpp"

SkyBox::SkyBox()
{
	drawBuffers = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 };

	for (int i = 0; i < cubeMapCount; ++i)
		CreateImage(i);

	TEXTURES->Read("grid.png");

	OBJECT_LOADER->Load(meshName.c_str(), "skyBox");
	mesh = MESHES->GetMesh("skyBox");
	mesh->SetShader("Skybox");
	mesh->SetDrawType(DrawType::Skybox);
	obj = new Object("Skybox");
	obj->AddComponent(new Transform);
	obj->AddComponent(mesh);
	OBJECTMANAGER->Add("Skybox", obj);
}

void SkyBox::CreateImage(int index)
{
	std::string file = cubeSideName[index] + ".jpg";
	TEXTURES->Read(file);

	skyBoxTexture.insert(std::make_pair(cubeSideName[index], TEXTURES->Get(file)));
}

void SkyBox::Draw()
{
	auto shader = mesh->GetShader();
	// top
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skyBoxTexture["top"].GetBufferObject());
	shader->Set("top", 0);

	// bottom
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, skyBoxTexture["bottom"].GetBufferObject());
	shader->Set("bottom", 1);

	// right
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, skyBoxTexture["right"].GetBufferObject());
	shader->Set("right", 2);

	// left
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, skyBoxTexture["left"].GetBufferObject());
	shader->Set("left", 3);

	// back
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, skyBoxTexture["back"].GetBufferObject());
	shader->Set("back", 4);

	// front
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, skyBoxTexture["front"].GetBufferObject());
	shader->Set("front", 5);
}

