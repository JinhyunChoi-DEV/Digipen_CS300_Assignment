/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Assignment1Stage.cpp
Purpose: Setup scene for Assignment1 requires
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include <GLFW/glfw3.h>
#include <filesystem>
#include <iostream>
#include <imgui/imgui.h>

#include "Input.hpp"
#include "Assignment1Stage.hpp"
#include "MeshManager.hpp"
#include "Object.hpp"
#include "ObjectManager.hpp"
#include "OrbitSphere.hpp"
#include "SphereMesh.hpp"
#include "Transform.hpp"
#include "Graphic.hpp"

Assignment1Stage::Assignment1Stage()
{
	sphereMeshName = "Default_Sphere";
	orbitLineName = "Line_Orbit";
	objectLoader = new ObjectLoader();
	orbitRadius = 1.0f;
	lastUpdateTime = 0.0f;
	pi = glm::pi<float>();

	GRAPHIC->CompileShader("Default.vert", "Default.frag", "Default");
	GRAPHIC->CompileShader("Line.vert", "Line.frag", "Line");
}

void Assignment1Stage::Initialize()
{
	CreateCamera();
	CreateOrbit();
	CreateSpheres();
	LoadAllObjects();
	CreateObject();
}

void Assignment1Stage::Update()
{
	float time = (float)glfwGetTime();
	float dt = time - lastUpdateTime;
	OBJECTMANAGER->GetObject("MainObject")->GetComponent<Transform>()->SetRotation(time *10, Y);

	UpdateCamera(dt);
	UpdateSpheres(time);
	UpdateGUI();

	lastUpdateTime = time;
}

void Assignment1Stage::Terminate()
{
}

void Assignment1Stage::LoadAllObjects()
{
	std::string path = objectLoader->GetObjFileDir();
	for(const auto& entry : std::filesystem::directory_iterator(path))
	{
		auto filePath = entry.path().filename();
		auto fileFull= filePath.string();
		auto lastIndex = fileFull.find_last_of(".");
		auto file = fileFull.substr(0, lastIndex);
		loadFiles.push_back(file);
		objectLoader->Load(fileFull.c_str(), file);
	}
}

void Assignment1Stage::UpdateCamera(float dt)
{
	float moveSpeed = dt * 10;

	auto camera = GRAPHIC->GetCamera();
	if (INPUT->IsPressed(GLFW_KEY_W))
		camera->Move(moveSpeed, CameraMoveAxis::Front);
	if (INPUT->IsPressed(GLFW_KEY_S))
		camera->Move(moveSpeed, CameraMoveAxis::Back);
	if (INPUT->IsPressed(GLFW_KEY_D))
		camera->Move(moveSpeed, CameraMoveAxis::Right);
	if (INPUT->IsPressed(GLFW_KEY_A))
		camera->Move(moveSpeed, CameraMoveAxis::Left);
	if (INPUT->IsPressed(GLFW_KEY_R))
		camera->ResetPosition();
	if (INPUT->IsPressed(GLFW_KEY_Q))
		camera->SetYaw(moveSpeed * 2);
	if (INPUT->IsPressed(GLFW_KEY_E))
		camera->SetYaw(-moveSpeed * 2);
	if (INPUT->IsPressed(GLFW_KEY_Z))
		camera->SetPitch(moveSpeed * 2);
	if (INPUT->IsPressed(GLFW_KEY_C))
		camera->SetPitch(-moveSpeed * 2);
}

void Assignment1Stage::UpdateSpheres(float time)
{
	float step = 2 * pi / spheres.size();
	for (unsigned i = 0; i < spheres.size(); ++i)
	{
		float additionAngle = step * i;
		auto transform = spheres[i]->GetComponent<Transform>();
		float x = 5 * cosf(time + additionAngle);
		float y = 0;
		float z = 5 * sinf(time + additionAngle);

		transform->SetTranslate({ x, y , z });
	}
}

void Assignment1Stage::CreateCamera()
{
	camera = GRAPHIC->GetCamera();
	CameraOriginData data;
	data.position = { 0,5,15};
	data.rotate = Pitch;
	data.degree = -15;
	camera->SetOriginData(data);
}


void Assignment1Stage::CreateOrbit()
{
	auto orbitMesh = OrbitSphere(orbitLineName, orbitRadius, 50);
	auto meshOrbit = orbitMesh.GetMesh();
	MESHES->Add(meshOrbit->GetName(), meshOrbit);

	Object* orbit = new Object();
	orbit->AddComponent(meshOrbit);
	orbit->AddComponent(new Transform());
	orbit->GetComponent<Transform>()->SetTranslate(glm::vec3{ 0, 0, 0 });
	orbit->GetComponent<Transform>()->SetScale(glm::vec3{ 5, 5, 5 });
	orbitScale = orbit->GetComponent<Transform>()->GetScale();
	OBJECTMANAGER->Add("orbit", orbit);
}

void Assignment1Stage::CreateObject()
{
	auto object = new Object;
	object->AddComponent(new Mesh(MESHES->GetMesh(loadFiles[0])));
	object->AddComponent(new Transform);
	OBJECTMANAGER->Add("MainObject", object);
	mainObject = object;
	selectedObject = loadFiles[0];
	mainObject->GetComponent<Transform>()->SetTranslate(glm::vec3(0));
	mainObject->GetComponent<Transform>()->SetScale(glm::vec3(1));
}

void Assignment1Stage::CreateSpheres()
{
	glm::vec3 scale = orbitScale;
	auto sphereMesh = SphereMesh(sphereMeshName, 1.0f, 25, 25);
	auto meshSphere = sphereMesh.GetMesh();
	MESHES->Add(meshSphere->GetName(), meshSphere);

	constexpr float pi = glm::pi<float>();
	float step = 2 * pi / 8;
	std::string baseName = "sphere";
	for (int i = 0; i < 8; ++i)
	{
		float angle = step * (float)i;
		float x = orbitRadius * cosf(angle);
		float y = 0.0f;
		float z = orbitRadius * sinf(angle);

		Object* object = new Object();
		object->AddComponent(meshSphere);
		object->AddComponent(new Transform());
		object->GetComponent<Transform>()->SetTranslate({ x*scale.x, y*scale.y, z* scale.z});
		object->GetComponent<Transform>()->SetScale(glm::vec3{ 0.2, 0.2, 0.2 });
		OBJECTMANAGER->Add(baseName + std::to_string(i), object);
		spheres.push_back(object);
	}
}

void Assignment1Stage::UpdateGUI()
{
	std::string test;
	auto mesh = mainObject->GetComponent<Mesh>();

	if (ImGui::BeginCombo("Loaded Objects", selectedObject.c_str()))
	{
		for(unsigned i =0; i < loadFiles.size(); ++i)
		{
			bool isSelected = selectedObject == loadFiles[i];
			if(ImGui::Selectable(loadFiles[i].c_str(), isSelected))
			{
				mainObject->DeleteComponent(mesh);
				delete mesh;
				selectedObject = loadFiles[i];
				mainObject->AddComponent(new Mesh(MESHES->GetMesh(selectedObject)));
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
}
