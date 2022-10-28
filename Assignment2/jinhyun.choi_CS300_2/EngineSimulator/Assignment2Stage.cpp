/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Assignment2Stage.cpp
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
#include "Assignment2Stage.hpp"
#include "MeshManager.hpp"
#include "Object.hpp"
#include "ObjectManager.hpp"
#include "OrbitSphere.hpp"
#include "SphereMesh.hpp"
#include "Transform.hpp"
#include "Graphic.hpp"
#include "Shader.hpp"

Assignment2Stage::Assignment2Stage()
{
	sphereMeshName = "Default_Sphere";
	orbitLineName = "Line_Orbit";
	objectLoader = new ObjectLoader();
	orbitRadius = 1.0f;
	lastUpdateTime = 0.0f;
	drawVertex = false;
	drawFace = false;
	pi = glm::pi<float>();

	GRAPHIC->CompileShader("Solid", "Solid.vert", "Solid.frag", "TransformModel.glsl", nullptr);
	GRAPHIC->CompileShader("PhongLighting", "PhongLighting.vert", "PhongLighting.frag", "LightingFunctions.glsl", "TransformModel.glsl", "Light.glsl", nullptr);
	GRAPHIC->CompileShader("BlinnShading", "BlinnShading.vert", "BlinnShading.frag", "LightingFunctions.glsl", "TransformModel.glsl", "Light.glsl", nullptr);
	GRAPHIC->CompileShader("PhongShading", "PhongShading.vert", "PhongShading.frag", "LightingFunctions.glsl", "TransformModel.glsl", "Light.glsl", nullptr);
	GRAPHIC->CompileShader("Line", "Line.vert", "Line.frag", "TransformModel.glsl", nullptr);

	reloadingShaderNames.insert(reloadingShaderNames.begin(), { "PhongLighting" , "BlinnShading", "PhongShading" });
}

void Assignment2Stage::Initialize()
{
	CreateCamera();
	CreateLightObject();
	CreateLightBall();
	LoadAllObjects();
	CreateObject();
	selectedShader = reloadingShaderNames[0];
}

void Assignment2Stage::Update()
{
	float time = (float)glfwGetTime();
	float dt = time - lastUpdateTime;
	OBJECTMANAGER->GetObject("MainObject")->GetComponent<Transform>()->SetRotation(time *10, Y);

	UpdateCamera(dt);
	UpdateSpheres(time);
	UpdateGUI();

	lastUpdateTime = time;
}

void Assignment2Stage::Terminate()
{
}

void Assignment2Stage::LoadAllObjects()
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

void Assignment2Stage::UpdateCamera(float dt)
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

void Assignment2Stage::UpdateSpheres(float time)
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

void Assignment2Stage::CreateCamera()
{
	camera = GRAPHIC->GetCamera();
	CameraOriginData data;
	data.position = { 0,5,15};
	data.rotate = Pitch;
	data.degree = -15;
	camera->SetOriginData(data);
}


void Assignment2Stage::CreateLightObject()
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

void Assignment2Stage::CreateObject()
{
	auto main = new Object;
	main->AddComponent(new Mesh(MESHES->GetMesh(loadFiles[0])));
	main->AddComponent(new Transform);
	OBJECTMANAGER->Add("MainObject", main);
	mainObject = main;
	selectedObject = loadFiles[0];
	mainObject->GetComponent<Transform>()->SetTranslate(glm::vec3(0));
	mainObject->GetComponent<Transform>()->SetScale(glm::vec3(1));

	auto floor = new Object;
	floor->AddComponent(new Mesh(MESHES->GetMesh("quad")));
	floor->AddComponent(new Transform);
	OBJECTMANAGER->Add("FloorObject", floor);
	floorObject = floor;
	floorObject->GetComponent<Transform>()->SetTranslate(glm::vec3(0, -1, 0));
	floorObject->GetComponent<Transform>()->SetScale(glm::vec3(5));
	floorObject->GetComponent<Transform>()->SetRotation(-90, X);
}

void Assignment2Stage::CreateLightBall()
{
	int objectCount = 1;
	glm::vec3 scale = orbitScale;
	auto sphereMesh = SphereMesh(sphereMeshName, 1.0f, 25, 25);
	auto meshSphere = sphereMesh.GetMesh();
	meshSphere->SetDrawType(Light);
	MESHES->Add(meshSphere->GetName(), meshSphere);

	constexpr float pi = glm::pi<float>();
	float step = 2 * pi / (float)objectCount;
	std::string baseName = "sphere";
	for (int i = 0; i < objectCount; ++i)
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

void Assignment2Stage::ModelsGUI()
{
	if (ImGui::TreeNode("Models"))
	{
		auto mesh = mainObject->GetComponent<Mesh>();

		if (ImGui::BeginCombo("Loaded Objects", selectedObject.c_str()))
		{
			for (unsigned i = 0; i < loadFiles.size(); ++i)
			{
				bool isSelected = selectedObject == loadFiles[i];
				if (ImGui::Selectable(loadFiles[i].c_str(), isSelected))
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
		ImGui::TreePop();
	}
}

void Assignment2Stage::NormalDrawGUI()
{
	if (ImGui::TreeNode("DrawNormal"))
	{
		ImGui::Checkbox("Draw Vertex Normal", &drawVertex);
		ImGui::Checkbox("Draw Face Normal", &drawFace);

		GRAPHIC->SetDrawNormal(drawVertex, drawFace);
		ImGui::TreePop();
	}
}

void Assignment2Stage::ReloadShaderGUI()
{
	if (ImGui::TreeNode("Shader"))
	{
		if (ImGui::BeginCombo("Shader Lists", selectedShader.c_str()))
		{
			for(int i = 0; i < reloadingShaderNames.size(); ++i)
			{
				bool isSelected = selectedShader == reloadingShaderNames[i];
				if(ImGui::Selectable(reloadingShaderNames[i].c_str(), isSelected))
				{
					selectedShader = reloadingShaderNames[i];
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		if(ImGui::Button("Reload Shader"))
		{
			std::string currentShader = mainObject->GetComponent<Mesh>()->GetShader()->Name;
			if(currentShader != selectedShader)
			{
				mainObject->GetComponent<Mesh>()->SetShader(selectedShader);
				floorObject->GetComponent<Mesh>()->SetShader(selectedShader);
			}
		}

		ImGui::TreePop();
	}
}

void Assignment2Stage::UpdateGUI()
{
	ModelsGUI();
	NormalDrawGUI();
	ReloadShaderGUI();
}
