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
#include "Mesh.hpp"
#include "Transform.hpp"
#include "Object.hpp"
#include "ObjectManager.hpp"
#include "Shader.hpp"

Graphic* GRAPHIC;

Graphic::Graphic()
{
	GRAPHIC = this;
	camera = new Camera();
	shaderManager = new ShaderManager();
	vertexObjectManager = new VertexObjectManager();
	drawVertexNormal = false;
	drawFaceNormal = false;
}

void Graphic::Initialize()
{
	glEnable(GL_DEPTH_TEST);
}

void Graphic::Update()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawNormalGUI();
	camera->Update();
	Draw();

	glFinish();
}

void Graphic::Terminate()
{

}

void Graphic::SetViewSize(glm::vec2 windowSize)
{
	glViewport(0, 0, static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
	camera->SetViewSize(windowSize);
}

void Graphic::SetCamera(Camera* camera)
{
	this->camera = camera;
}

void Graphic::CompileShader(std::string vertexFile, std::string fragFile, std::string name)
{
	auto vertex = vertexFile.c_str();
	auto fragment = fragFile.c_str();
	shaderManager->Compile(vertex, fragment, name);
}

Shader* Graphic::GetShader(std::string name) const
{
	return shaderManager->GetShader(name);
}

void Graphic::Draw()
{
	const auto objects = OBJECTMANAGER->GetObjects();
	if (objects.empty())
		return;

	for (auto pair : objects)
	{
		auto object = pair.second;
		auto meshData = object->GetComponent<Mesh>();
		auto transformData = object->GetComponent<Transform>();

		if(meshData == nullptr)
			continue;

		if (transformData == nullptr)
		{
			transformData = new Transform();
			object->AddComponent(transformData);
		}

		DrawType type = meshData->GetType();

		if (type == ObjectModel)
		{
			DrawObject(transformData, meshData);

			if (drawVertexNormal)
				DrawVertexNormal(transformData, meshData);

			if (drawFaceNormal)
				DrawFaceNormal(transformData, meshData);
		}

		if (type == Solid)
			DrawObject(transformData, meshData);

		if (type == Line)
			DrawLine(transformData, meshData);
	}
}

void Graphic::DrawObject(const Transform* transform, const Mesh* mesh)
{
	auto VAO = vertexObjectManager->GetObjectVAO(mesh);

	if (VAO == 0)
		return;

	auto shader = mesh->GetShader();
	auto model = transform->GetMatrix();
	auto view = camera->GetView();
	auto projection = camera->GetProjection();

	shader->Use();
	shader->Set("model", model);
	shader->Set("view", view);
	shader->Set("projection", projection);
	shader->Set("objectColor", glm::vec3(0.8, 0.8, 0.8));
	shader->Set("lightColor", glm::vec3(1, 1, 1));
	shader->Set("lightPosition", {0, 5, 3});

	glBindVertexArray(VAO);

	if(mesh->GetIsMultipleFaceIndex())
		glDrawElements(GL_TRIANGLE_FAN, static_cast<int>(mesh->GetIndices().size()), GL_UNSIGNED_INT, nullptr);
	else
		glDrawElements(GL_TRIANGLES, static_cast<int>(mesh->GetIndices().size()), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}

void Graphic::DrawLine(const Transform* transform, const Mesh* mesh)
{
	auto VAO = vertexObjectManager->GetObjectVAO(mesh);

	if (VAO == 0)
		return;

	auto shader = mesh->GetShader();
	auto model = transform->GetMatrix();
	auto view = camera->GetView();
	auto projection = camera->GetProjection();

	shader->Use();
	shader->Set("model", model);
	shader->Set("view", view);
	shader->Set("projection", projection);
	shader->Set("lineColor", glm::vec3(1,1,1));

	auto vertices = mesh->GetPositions();
	glBindVertexArray(VAO);

	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices.size()));

	glBindVertexArray(0);
}

void Graphic::DrawVertexNormal(Transform* transform, const Mesh* mesh)
{
	auto VAO = vertexObjectManager->GetVertexNormalVAO(mesh);

	if (VAO == 0)
		return;

	auto shader = GetShader("Line");
	auto model = transform->GetMatrix();
	auto view = camera->GetView();
	auto projection = camera->GetProjection();

	shader->Use();
	shader->Set("model", model);
	shader->Set("view", view);
	shader->Set("projection", projection);
	shader->Set("lineColor", glm::vec3(0, 0, 1));

	auto normalLines = mesh->GetVertexNormalLines();
	glBindVertexArray(VAO);

	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(normalLines.size()));

	glBindVertexArray(0);
}

void Graphic::DrawFaceNormal(Transform* transform, const Mesh* mesh)
{
	auto VAO = vertexObjectManager->GetFaceNormalVAO(mesh);

	if (VAO == 0)
		return;

	auto shader = GetShader("Line");
	auto model = transform->GetMatrix();
	auto view = camera->GetView();
	auto projection = camera->GetProjection();

	shader->Use();
	shader->Set("model", model);
	shader->Set("view", view);
	shader->Set("projection", projection);
	shader->Set("lineColor", glm::vec3(0, 1, 0));

	auto normalLines = mesh->GetFaceNormalLines();
	glBindVertexArray(VAO);

	glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(normalLines.size()));

	glBindVertexArray(0);
}

void Graphic::DrawNormalGUI()
{
	ImGui::Checkbox("Draw Vertex Normal", &drawVertexNormal);
	ImGui::Checkbox("Draw Face Normal", &drawFaceNormal);
}

