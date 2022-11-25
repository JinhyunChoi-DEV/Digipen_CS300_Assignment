/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Graphic.cpp
Purpose: Making graphic core system about drawing actions
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <iostream>

#include "Graphic.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "Object.hpp"
#include "ObjectManager.hpp"
#include "Shader.hpp"
#include "Skybox.hpp"
#include "EnvironmentMapping.hpp"

Graphic* GRAPHIC;

Graphic::Graphic()
{
	GRAPHIC = this;
	baseCamera = new Camera();
	shaderManager = new ShaderManager();
	vertexObjectManager = new VertexObjectManager();
	uboManager = new UniformBlockObjectManager();
	drawVertexNormal = false;
	drawFaceNormal = false;

	//pitch, yaw
	sides[DirectSide::Top] = std::make_pair(90.0f, 0.0f);
	sides[DirectSide::Bottom] = std::make_pair(-90.0f, 0.0f);
	sides[DirectSide::Front] = std::make_pair(0.0f, 0.0f);
	sides[DirectSide::Back] = std::make_pair(0.0f, 180.0f);
	sides[DirectSide::Left] = std::make_pair(0.0f, 90.0f);
	sides[DirectSide::Right] = std::make_pair(0.0f, -90.0f);
}

void Graphic::Initialize()
{
	glEnable(GL_DEPTH_TEST);

	uboManager->InitializeTransform();
	uboManager->InitializeLight();
	environmentMapping = new EnvironmentMapping(windowSize);
	environmentFBOCamera = new Camera();
	environmentFBOCamera->SetRatio(1);
	environmentFBOCamera->SetFOV(90.0f);
}

void Graphic::Update()
{
	if (windowSize.x <= 0 || windowSize.y <= 0)
		return;

	UpdateLight();

	auto environmentObjects = OBJECTMANAGER->GetEnvironmentObjects();
	renderTargetCam = environmentFBOCamera;
	DrawEnvironment(environmentObjects);

	auto objects = OBJECTMANAGER->GetObjects();
	renderTargetCam = baseCamera;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (const auto& object : objects)
		DrawByType(object.second);

	glFinish();
}

void Graphic::Terminate()
{ }

void Graphic::SetViewSize(glm::vec2 windowSize)
{
	this->windowSize = windowSize;
	glViewport(0, 0, static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
	baseCamera->SetViewSize(windowSize);
}

void Graphic::SetCamera(Camera* camera)
{
	this->baseCamera = camera;
}

void Graphic::CompileShader(std::string name, const char* path, ...)
{
	std::vector<const char*> shaderPaths;
	va_list argList;
	va_start(argList, path);

	while (path != nullptr)
	{
		shaderPaths.push_back(path);
		path = va_arg(argList, const char*);
	}
	va_end(argList);

	shaderManager->Compile(name, shaderPaths);
}

void Graphic::SetDrawNormal(bool drawVertex, bool drawFace)
{
	drawVertexNormal = drawVertex;
	drawFaceNormal = drawFace;
}

void Graphic::RebindTexture(Object* object)
{
	auto mesh = object->GetComponent<Mesh>();
	auto texture = object->GetComponent<Texture>();

	vertexObjectManager->RebindTexture(mesh, texture);
}

void Graphic::SetGlobalLightInfo(glm::vec3 attenuation, glm::vec3 globalAmbient, glm::vec3 fog, float near, float far)
{
	attenuationConstants = attenuation;
	globalAmbientColor = globalAmbient;
	fogColor = fog;
	fogMin = near;
	fogMax = far;
}

void Graphic::SetSkyBox(SkyBox* skyBox)
{
	this->skyBox = skyBox;
}

Shader* Graphic::GetShader(std::string name) const
{
	return shaderManager->GetShader(name);
}

void Graphic::UpdateLight()
{
	const auto objects = OBJECTMANAGER->GetLights();
	if (objects.empty())
		return;

	uboManager->BindLightData(objects, attenuationConstants, globalAmbientColor, fogColor, fogMin, fogMax);
}

void Graphic::DrawEnvironment(std::vector<Object*> objects)
{
	for (auto side : sides)
	{
		environmentMapping->Bind((int)side.first);
		environmentFBOCamera->ResetPosition();
		environmentFBOCamera->SetPitch(side.second.first);
		environmentFBOCamera->SetYaw(side.second.second);

		for (auto& object : objects)
			DrawByType(object);

		environmentMapping->UnBind();
	}
}

void Graphic::DrawByType(Object* object)
{
	auto mesh = object->GetComponent<Mesh>();
	if (!object->IsActive())
		return;

	if (mesh == nullptr)
		return;

	DrawType type = mesh->GetType();

	if (type == DrawType::ObjectModel)
		DrawModel(object);

	if (type == DrawType::Solid)
		DrawSolid(object);

	if (type == DrawType::Line)
		DrawLine(object);

	if (type == DrawType::Light)
	{
		auto lightData = object->GetComponent<Light>();
		if (lightData == nullptr)
			return;

		DrawLight(object);
	}

	if (type == DrawType::Skybox)
		DrawSkybox();
}

void Graphic::DrawModel(Object* object)
{
	if (object->isEnvironmentMappingTarget)
		DrawEnvironmentTarget(object);
	else
		DrawObject(object);

	if (drawVertexNormal)
		DrawVertexNormal(object);

	if (drawFaceNormal)
		DrawFaceNormal(object);
}

void Graphic::DrawObject(Object* object)
{
	auto transform = object->GetComponent<Transform>();
	auto mesh = object->GetComponent<Mesh>();
	auto texture = object->GetComponent<Texture>();

	auto VAO = vertexObjectManager->GetObjectVAO(mesh);

	if (VAO == 0)
		return;

	auto shader = mesh->GetShader();

	shader->Use();
	uboManager->BindTransformData(transform, renderTargetCam->GetView(), renderTargetCam->GetProjection());
	TextureBind(mesh, texture);
	if (texture != nullptr)
		texture->Bind(mesh);

	shader->Set("cameraPos", baseCamera->GetPosition());

	glBindVertexArray(VAO);

	if (mesh->GetIsMultipleFaceIndex())
		glDrawElements(GL_TRIANGLE_FAN, static_cast<int>(mesh->GetIndices().size()), GL_UNSIGNED_INT, nullptr);
	else
		glDrawElements(GL_TRIANGLES, static_cast<int>(mesh->GetIndices().size()), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}

void Graphic::DrawSolid(Object* object)
{
	auto transform = object->GetComponent<Transform>();
	auto mesh = object->GetComponent<Mesh>();

	auto VAO = vertexObjectManager->GetObjectVAO(mesh);

	if (VAO == 0)
		return;

	auto shader = mesh->GetShader();
	shader->Use();
	uboManager->BindTransformData(transform, renderTargetCam->GetView(), renderTargetCam->GetProjection());
	shader->Set("vertexColor", glm::vec3(0.5, 0.5, 0.5));

	glBindVertexArray(VAO);

	if (mesh->GetIsMultipleFaceIndex())
		glDrawElements(GL_TRIANGLE_FAN, static_cast<int>(mesh->GetIndices().size()), GL_UNSIGNED_INT, nullptr);
	else
		glDrawElements(GL_TRIANGLES, static_cast<int>(mesh->GetIndices().size()), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}

void Graphic::DrawLine(Object* object)
{
	auto transform = object->GetComponent<Transform>();
	auto mesh = object->GetComponent<Mesh>();

	auto VAO = vertexObjectManager->GetObjectVAO(mesh);

	if (VAO == 0)
		return;

	auto shader = mesh->GetShader();
	shader->Use();
	uboManager->BindTransformData(transform, renderTargetCam->GetView(), renderTargetCam->GetProjection());
	shader->Set("lineColor", glm::vec3(1, 1, 1));

	auto vertices = mesh->GetPositions();
	glBindVertexArray(VAO);

	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices.size()));

	glBindVertexArray(0);
}

void Graphic::DrawLight(Object* object)
{
	auto transform = object->GetComponent<Transform>();
	auto mesh = object->GetComponent<Mesh>();
	auto light = object->GetComponent<Light>();

	auto VAO = vertexObjectManager->GetObjectVAO(mesh);

	if (VAO == 0)
		return;

	auto shader = mesh->GetShader();
	auto color = light->GetDiffuseIntensity();
	shader->Use();
	uboManager->BindTransformData(transform, renderTargetCam->GetView(), renderTargetCam->GetProjection());
	shader->Set("vertexColor", color);

	glBindVertexArray(VAO);

	if (mesh->GetIsMultipleFaceIndex())
		glDrawElements(GL_TRIANGLE_FAN, static_cast<int>(mesh->GetIndices().size()), GL_UNSIGNED_INT, nullptr);
	else
		glDrawElements(GL_TRIANGLES, static_cast<int>(mesh->GetIndices().size()), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}

void Graphic::DrawVertexNormal(Object* object)
{
	auto transform = object->GetComponent<Transform>();
	auto mesh = object->GetComponent<Mesh>();

	auto VAO = vertexObjectManager->GetVertexNormalVAO(mesh);

	if (VAO == 0)
		return;

	auto shader = GetShader("Line");

	shader->Use();
	uboManager->BindTransformData(transform, baseCamera->GetView(), baseCamera->GetProjection());
	shader->Set("lineColor", glm::vec3(0, 0, 1));

	auto normalLines = mesh->GetVertexNormalLines();
	glBindVertexArray(VAO);

	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(normalLines.size()));

	glBindVertexArray(0);
}

void Graphic::DrawFaceNormal(Object* object)
{
	auto transform = object->GetComponent<Transform>();
	auto mesh = object->GetComponent<Mesh>();

	auto VAO = vertexObjectManager->GetFaceNormalVAO(mesh);

	if (VAO == 0)
		return;

	auto shader = GetShader("Line");

	shader->Use();
	uboManager->BindTransformData(transform, baseCamera->GetView(), baseCamera->GetProjection());
	shader->Set("lineColor", glm::vec3(0, 1, 0));

	auto normalLines = mesh->GetFaceNormalLines();
	glBindVertexArray(VAO);

	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(normalLines.size()));

	glBindVertexArray(0);
}

void Graphic::TextureBind(Mesh* mesh, Texture* texture)
{
	if (texture == nullptr)
	{
		auto shader = mesh->GetShader();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, -1);
		shader->Set("ambientTexture", 0);

		// specular
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, -1);
		shader->Set("specularTexture", 1);

		// diffuse
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, -1);
		shader->Set("diffuseTexture", 2);

		// emissive
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, -1);
		shader->Set("emissiveTexture", 3);

		shader->Set("useAmbientTexture", false);
		shader->Set("useSpecularTexture", false);
		shader->Set("useDiffuseTexture", false);
		shader->Set("useEmissiveTexture", false);
		shader->Set("useTexture", false);
		return;
	}

	texture->Bind(mesh);
}

void Graphic::DrawSkybox()
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	auto object = OBJECTMANAGER->GetObject("Skybox");
	auto mesh = object->GetComponent<Mesh>();
	auto transform = object->GetComponent<Transform>();
	auto VAO = vertexObjectManager->GetObjectVAO(mesh);
	auto shader = mesh->GetShader();

	shader->Use();
	skyBox->Draw();
	auto view = glm::mat4(glm::mat3(renderTargetCam->GetView()));
	uboManager->BindTransformData(transform, view, renderTargetCam->GetProjection());

	glBindVertexArray(VAO);

	if (mesh->GetIsMultipleFaceIndex())
		glDrawElements(GL_TRIANGLE_FAN, static_cast<int>(mesh->GetIndices().size()), GL_UNSIGNED_INT, nullptr);
	else
		glDrawElements(GL_TRIANGLES, static_cast<int>(mesh->GetIndices().size()), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

void Graphic::DrawEnvironmentTarget(Object* object)
{
	auto transform = object->GetComponent<Transform>();
	auto mesh = object->GetComponent<Mesh>();
	auto shader = mesh->GetShader();

	shader->Use();
	auto VAO = vertexObjectManager->GetObjectVAO(mesh);
	environmentMapping->UpdateMappingTexture(shader);
	shader->Set("viewPos", renderTargetCam->GetPosition());

	uboManager->BindTransformData(transform, renderTargetCam->GetView(), renderTargetCam->GetProjection());
	glBindVertexArray(VAO);

	if (mesh->GetIsMultipleFaceIndex())
		glDrawElements(GL_TRIANGLE_FAN, static_cast<int>(mesh->GetIndices().size()), GL_UNSIGNED_INT, nullptr);
	else
		glDrawElements(GL_TRIANGLES, static_cast<int>(mesh->GetIndices().size()), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}
