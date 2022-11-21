#include <glad/glad.h>
#include "Skybox.hpp"
#include "MeshManager.hpp"
#include "Object.hpp"
#include "ObjectLoader.hpp"
#include "ObjectManager.hpp"
#include "TextureManager.hpp"
#include "Transform.hpp"

SkyBox::SkyBox(glm::vec2 windowSize)
{
	width = windowSize.x;
	height = windowSize.y;

	for(int i = 0; i< cubeMapCount; ++i)
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
	
	//CreateFrameBuffer();
	//CreateRenderToTexture();
}

void SkyBox::CreateImage(int index)
{
	std::string file = cubeSideName[index] + ".jpg";
	TEXTURES->Read(file);

	skyBoxTexture.insert(std::make_pair(cubeSideName[index], TEXTURES->Get(file)));
}

void SkyBox::CreateFrameBuffer()
{
	// handle to framebuffer, create the FB object handle
	if (frameBuffer != 0)
		glDeleteFramebuffers(1, &frameBuffer);

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

void SkyBox::CreateRenderToTexture()
{
	drawBuffers.clear();
	auto temp = *skyBoxTexture.begin();

	if (renderedTexture != 0)
		glDeleteTextures(1, &renderedTexture);
	// the texture we are going to render to
	glGenTextures(1, &renderedTexture);
	// bind the newly created texture, all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	//give an empty image to OpenGL
	GLint type = 0;
	if (temp.second.GetChannel() == 3)
		type = GL_RGB;
	else if (temp.second.GetChannel() == 4)
		type = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, nullptr);


	if (depthRenderBuffer != 0)
		glDeleteRenderbuffers(1, &depthRenderBuffer);
	// Add Depth Buffer
	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (int)windowSize.x, (int)windowSize.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
	drawBuffers.push_back(GL_COLOR_ATTACHMENT0);
}

void SkyBox::Draw()
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	auto shader = mesh->GetShader();

	// top
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skyBoxTexture["top"].GetBufferObject());
	//glBindTexture(GL_TEXTURE_2D, TEXTURES->Get("grid.png").GetBufferObject());
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
