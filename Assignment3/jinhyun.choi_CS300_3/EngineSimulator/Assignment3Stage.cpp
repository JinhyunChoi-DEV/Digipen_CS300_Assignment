/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Assignment3Stage.cpp
Purpose: the source file for Assignment3Stage data and function
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/27/2022
End Header --------------------------------------------------------*/

#include <filesystem>
#include <iostream>
#include <glm/ext/scalar_constants.hpp>

#include "Assignment3Stage.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include "LightTypeHelper.hpp"
#include "ObjectLoader.hpp"
#include "ObjectManager.hpp"
#include "Object.hpp"
#include "SphereMesh.hpp"
#include "Graphic.hpp"
#include "Skybox.hpp"
#include "EnvironmentMapping.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "MeshManager.hpp"

Assignment3Stage::Assignment3Stage()
{
	pauseRotation = false;
	sphereMeshName = "Default_Sphere";
	pi = glm::pi<float>();
	rotationTime = 0.0f;
	activeLightCount = 1;
	drawFace = false;
	drawVertex = false;

	refractionFactor =
	{
		std::make_pair("Air", 1.0003f),
		std::make_pair("Hydrogen", 1.0001f),
		std::make_pair("Water", 1.333f),
		std::make_pair("Olive Oil", 1.47f),
		std::make_pair("Ice", 1.31f),
		std::make_pair("Quartz", 1.46f),
		std::make_pair("Diamond", 2.42f),
		std::make_pair("Acrylic ", 1.49f),
		std::make_pair("Plexiglas ", 1.49f),
		std::make_pair("Lucite ", 1.49f),
	};

	GRAPHIC->CompileShader("FrameBuffer", "FrameBuffer.vert", "FrameBuffer.frag", "TransformModel.glsl", nullptr);
	GRAPHIC->CompileShader("EnvironmentMapping", "EnvironmentMapping.vert", "EnvironmentMapping.frag", "TransformModel.glsl", "Texture.glsl", "Light.glsl", nullptr);
	GRAPHIC->CompileShader("Solid", "Solid.vert", "Solid.frag", "TransformModel.glsl", nullptr);
	GRAPHIC->CompileShader("Skybox", "Skybox.vert", "Skybox.frag", "TransformModel.glsl", nullptr);
	GRAPHIC->CompileShader("Line", "Line.vert", "Line.frag", "TransformModel.glsl", nullptr);
}

void Assignment3Stage::Initialize()
{
	skyBox = new SkyBox();
	GRAPHIC->SetSkyBox(skyBox);
	materialsIndex = 0;
	selectedMaterials = refractionFactor[0].first;
	GRAPHIC->GetEnvironmentMapping()->refractiveIndex = refractionFactor[0].second;

	LoadAllObjects();
	CreateMainObject();
	CreateLightBall();

	auto camera = GRAPHIC->GetCamera();
	CameraOriginData data;
	data.position = { 0,2.5f,15 };
	camera->SetOriginData(data);
}

void Assignment3Stage::Update()
{
	float time = (float)glfwGetTime();
	float dt = time - lastUpdateTime;

	UpdateCamera(dt);

	if (!pauseRotation)
		UpdateLightBall(dt);

	UpdateGUI();

	lastUpdateTime = time;
}

void Assignment3Stage::UpdateCamera(float dt)
{
	float moveSpeed = dt * 10;

	auto camera = GRAPHIC->GetCamera();
	if (INPUT->IsPressed(GLFW_KEY_W))
		camera->Move(moveSpeed * 2, CameraMoveAxis::Front);
	if (INPUT->IsPressed(GLFW_KEY_S))
		camera->Move(moveSpeed * 2, CameraMoveAxis::Back);
	if (INPUT->IsPressed(GLFW_KEY_D))
		camera->Move(moveSpeed * 2, CameraMoveAxis::Right);
	if (INPUT->IsPressed(GLFW_KEY_A))
		camera->Move(moveSpeed * 2, CameraMoveAxis::Left);
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

void Assignment3Stage::UpdateLightBall(float time)
{
	rotationTime += time;
	auto lights = OBJECTMANAGER->GetLights();
	float step = 2 * pi / lights.size();

	for (unsigned i = 0; i < lights.size(); ++i)
	{
		float additionAngle = step * i;
		auto transform = lights[i]->GetComponent<Transform>();
		float x = 5.0f * cosf(rotationTime + additionAngle);
		float y = 0;
		float z = 5.0f * sinf(rotationTime + additionAngle);
		transform->SetTranslate({ x, y , z });

		auto light = lights[i]->GetComponent<Light>();
		auto mainTransform = mainObject->GetComponent<Transform>();

		auto newDir = mainTransform->GetPosition() - transform->GetPosition();
		light->SetDirection(newDir);
	}
}

void Assignment3Stage::LoadAllObjects()
{
	std::string path = OBJECT_LOADER->GetObjFileDir();
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		auto filePath = entry.path().filename();
		auto fileFull = filePath.string();
		auto lastIndex = fileFull.find_last_of(".");
		auto file = fileFull.substr(0, lastIndex);
		loadFiles.push_back(file);
		OBJECT_LOADER->Load(fileFull.c_str(), file);
	}
}

void Assignment3Stage::CreateMainObject()
{
	auto texture = new Texture();
	texture->SetMappingType(TextureMappingType::Cube);
	texture->SetDiffuse("metal_roof_diff_512x512.png");
	texture->SetAmbientColor({ 0,0,0 });
	texture->SetDiffuseColor({ 1,1,1 });
	texture->SetSpecularColor({ 1,1,1 });
	texture->SetEntityType(TextureEntityType::VertexPosition);
	texture->SetUseGPU(true);

	auto main = new Object("MainObject");
	main->AddComponent(new Mesh(MESHES->GetMesh("sphere")));
	main->AddComponent(new Transform);
	main->AddComponent(texture);
	OBJECTMANAGER->Add("MainObject", main);
	mainObject = main;
	selectedMesh = "sphere";
	selectedObject = "sphere";
	mainObject->isEnvironmentMappingTarget = true;
	mainObject->GetComponent<Mesh>()->SetShader("EnvironmentMapping");
	mainObject->GetComponent<Mesh>()->SetDrawType(DrawType::ObjectModel);
	mainObject->GetComponent<Transform>()->SetTranslate(glm::vec3(0));
	mainObject->GetComponent<Transform>()->SetScale(glm::vec3(1.5));
}

void Assignment3Stage::CreateLightBall()
{
	int objectCount = 16;
	auto sphereMesh = SphereMesh(sphereMeshName, 1.0f, 25, 25);
	auto meshSphere = sphereMesh.GetMesh();
	meshSphere->SetDrawType(DrawType::Light);
	MESHES->Add(meshSphere->GetName(), meshSphere);

	constexpr float pi = glm::pi<float>();
	float step = 2 * pi / (float)objectCount;
	std::string baseName = "Light";
	for (int i = 0; i < objectCount; ++i)
	{
		float angle = step * (float)i;
		float x = 5.0f * cosf(angle);
		float y = 0.0f;
		float z = 5.0f * sinf(angle);

		std::string name = baseName + std::to_string(i + 1);
		Object* object = new Object(name);
		object->AddComponent(meshSphere);
		object->AddComponent(new Light());

		if (activeLightCount <= i)
			object->SetActive(false);

		object->AddComponent(new Transform());
		object->GetComponent<Transform>()->SetTranslate({ x * 5.0f, y, z * 5.0f });
		object->GetComponent<Transform>()->SetScale(glm::vec3{ 0.2, 0.2, 0.2 });
		OBJECTMANAGER->Add(name, object);

		if (i == 0)
			selectedLight = name;

		auto light = object->GetComponent<Light>();
		light->SetType(LightType::Point);
		light->SetDirection({ 1,0,0 });
		light->SetInnerAngle(23.0f);
		light->SetOuterAngle(27.5f);
		lightBalls.push_back(object);
	}
}

void Assignment3Stage::UpdateGUI()
{
	ModelsGUI();
	NormalDrawGUI();
	LightingBallGUI();
	MaterialGUI();
	GlobalLightGUI();
}

void Assignment3Stage::ModelsGUI()
{
	if (ImGui::CollapsingHeader("Models"))
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
					auto mesh = new Mesh(MESHES->GetMesh(selectedObject));
					mesh->SetShader("EnvironmentMapping");
					mainObject->AddComponent(mesh);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
	}
}

void Assignment3Stage::NormalDrawGUI()
{
	if (ImGui::CollapsingHeader("DrawNormal"))
	{
		ImGui::Checkbox("Draw Vertex Normal", &drawVertex);
		ImGui::Checkbox("Draw Face Normal", &drawFace);

		GRAPHIC->SetDrawNormal(drawVertex, drawFace);
	}
}

void Assignment3Stage::LightingBallGUI()
{
	ImGui::Begin("Light Controls");

	auto selectedLightObject = OBJECTMANAGER->GetObject(selectedLight);
	auto lights = OBJECTMANAGER->GetLights();
	std::sort(lights.begin(), lights.end(), [](Object* a, Object* b)
		{
			if (a->GetName().size() == b->GetName().size())
				return a->GetName() < b->GetName();

			return a->GetName().size() < b->GetName().size();
		});

	if (ImGui::CollapsingHeader("Light Control"))
	{
		ImGui::Text("Light Object Count");
		if (ImGui::SliderInt("Count", &activeLightCount, 1, 16))
			SetActiveLightBalls(activeLightCount);

		ImGui::SeparatorEx(1);
		ImGui::Text("Orbit Rotate Action");
		ImGui::Checkbox("Pause Rotation", &pauseRotation);

		// Scenarios Buttons
		ImGui::SeparatorEx(1);
		ImGui::Text("Lighting Scenarios");
		if (ImGui::Button("Scenario 1"))
		{
			glm::vec3 color = { 1,1,1 };
			SetScenario1(5, color);
		}
		ImGui::SameLine();

		if (ImGui::Button("Scenario 2"))
		{
			std::vector<glm::vec3> colors =
			{
				{0.230f, 0.917f, 0.081f}, {0.086f, 0.350f, 0.971f,},
				{0.956f, 0.056f, 0.056f}, {0.795f, 0.113f, 0.887f},
				{0.113f, 0.887f, 0.705f}, {0.887f, 0.842f, 0.113f}, {0.887f, 0.409f, 0.113f}
			};
			SetScenario2(colors);
		}
		ImGui::SameLine();

		if (ImGui::Button("Scenario 3"))
		{
			SetScenario3(11);
		}
		ImGui::SameLine();

		// All Light Lists
		ImGui::NewLine();
		ImGui::Text("Light List");
		if (ImGui::BeginCombo("Selected Light", selectedLight.c_str()))
		{
			for (int i = 0; i < (int)lights.size(); ++i)
			{
				bool isSelected = selectedLight == lights[i]->GetName();
				if (ImGui::Selectable(lights[i]->GetName().c_str(), isSelected))
				{
					selectedLight = lights[i]->GetName();
					selectedLightObject = OBJECTMANAGER->GetObject(selectedLight);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		// Each Light Infos
		if (ImGui::TreeNode("Light Status"))
		{
			auto light = selectedLightObject->GetComponent<Light>();
			auto type = light->GetType();
			auto preview = GetLightTypeString(type);
			if (ImGui::BeginCombo("Type", preview.c_str()))
			{
				auto lightTypes = GetLightTypesString();
				for (int i = 0; i < (int)lightTypes.size(); ++i)
				{
					bool isSelected = preview == lightTypes[i];
					if (ImGui::Selectable(lightTypes[i].c_str(), isSelected))
					{
						type = GetTypeFromString(lightTypes[i]);
						light->SetType(type);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			// Color
			auto color = light->GetAmientIntensity();
			if (ImGui::ColorEdit3("Ambient", &color[0]))
			{
				light->SetAmientIntensity(color);
			}

			color = light->GetDiffuseIntensity();
			if (ImGui::ColorEdit3("Diffuse", &color[0]))
			{
				light->SetDiffuseIntensity(color);
			}

			color = light->GetSpecularIntensity();
			if (ImGui::ColorEdit3("Specular", &color[0]))
			{
				light->SetSpecularIntensity(color);
			}

			// Spotlight Data
			if (type == LightType::Spotlight)
			{
				auto innerAngle = light->GetInnerAngle();
				auto outerAngle = light->GetOuterAngle();
				auto fallout = light->GetFallOut();

				if (ImGui::SliderFloat("Inner Angle", &innerAngle, 0, 90))
				{
					if (innerAngle < outerAngle)
						light->SetInnerAngle(innerAngle);
				}

				if (ImGui::SliderFloat("Outer Angle", &outerAngle, 0, 90))
				{
					light->SetOuterAngle(outerAngle);
					if (innerAngle > outerAngle)
						light->SetInnerAngle(outerAngle);
				}

				if (ImGui::SliderFloat("Fallout", &fallout, 1, 30))
				{
					light->SetFallOut(fallout);
				}
			}

			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void Assignment3Stage::MaterialGUI()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::Text("Control Color Mix");
		ImGui::SliderFloat("Color Mix Ratio", &GRAPHIC->GetEnvironmentMapping()->colorMixRatio, 0.0f, 1.0f);

		ImGui::SeparatorEx(1);

		ImGui::Text("Surface Color Tints");
		auto color = mainObject->GetComponent<Texture>()->GetEmissiveColor();
		if (ImGui::ColorEdit3("Emissive", &color[0]))
		{
			mainObject->GetComponent<Texture>()->SetEmissiveColor(color);
		}

		ImGui::SeparatorEx(1);

		ImGui::Text("Environment Mapping");
		auto currentType = GetVisualizeTypeString(GRAPHIC->GetEnvironmentMapping()->visualizeType);
		auto listType = GetVisualizeTypeString();
		if (ImGui::BeginCombo("Mapping Mode", currentType.c_str()))
		{
			for (int i = 0; i < (int)listType.size(); ++i)
			{
				bool  isSelected = currentType == listType[i];
				if (ImGui::Selectable(listType[i].c_str(), isSelected))
				{
					GRAPHIC->GetEnvironmentMapping()->visualizeType = GetVisualizeTypeFromString(listType[i]);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		ImGui::SeparatorEx(1);

		ImGui::Text("Environment Mapping Flag Controls");
		if (ImGui::BeginCombo("Refraction Materials", selectedMaterials.c_str()))
		{
			for (int i = 0; i < (int)refractionFactor.size(); ++i)
			{
				bool isSelected = selectedMaterials == refractionFactor[i].first;
				if (ImGui::Selectable(refractionFactor[i].first.c_str(), isSelected))
				{
					materialsIndex = i;
					selectedMaterials = refractionFactor[i].first;
					GRAPHIC->GetEnvironmentMapping()->refractiveIndex = refractionFactor[i].second;
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		float current = GRAPHIC->GetEnvironmentMapping()->refractiveIndex;
		float base = refractionFactor[materialsIndex].second;
		if (ImGui::SliderFloat("Refraction Index", &current, 1.0f, 100.0f, "%.3f", 1) || INPUT->IsPressedMouseButton(GLFW_MOUSE_BUTTON_LEFT))
		{
			GRAPHIC->GetEnvironmentMapping()->refractiveIndex = current;
		}
		else
		{
			GRAPHIC->GetEnvironmentMapping()->refractiveIndex = base;
		}

		ImGui::SliderFloat("Fresnel Power", &GRAPHIC->GetEnvironmentMapping()->fresnelPower, 0.1f, 10.0f);
	}
}

void Assignment3Stage::GlobalLightGUI()
{
	if (ImGui::CollapsingHeader("Global Constant Control"))
	{
		auto attenuation = GRAPHIC->GetAttenuation();
		auto globalAmbient = GRAPHIC->GetGlobalAmbient();
		auto fog = GRAPHIC->GetFogColor();
		auto near = GRAPHIC->GetFogMin();
		auto far = GRAPHIC->GetFogMax();

		ImGui::DragFloat3("Attenuation Constant", &attenuation[0], 0.01f, 0.0f, 2.0f);
		ImGui::ColorEdit3("Global Ambient", &globalAmbient[0]);
		ImGui::ColorEdit3("Fog Color", &fog[0]);
		ImGui::DragFloat("Fog Min", &near, 0.1f, 0.1f, 20.0f);
		ImGui::DragFloat("Fog Max", &far, 0.1f, 0.1f, 50.0f);

		if (near > far)
			near = far;
		if (far < near)
			far = near;

		GRAPHIC->SetGlobalLightInfo(attenuation, globalAmbient, fog, near, far);
	}
}

void Assignment3Stage::SetActiveLightBalls(int count)
{
	activeLightCount = count;
	for (int i = 0; i < (int)lightBalls.size(); ++i)
	{
		if (activeLightCount <= i)
			lightBalls[i]->SetActive(false);
		else
			lightBalls[i]->SetActive(true);
	}
	selectedLight = lightBalls[0]->GetName();
}

void Assignment3Stage::SetScenario1(int count, glm::vec3 color)
{
	SetActiveLightBalls(count);

	auto lights = OBJECTMANAGER->GetLights();
	for (int i = 0; i < count; ++i)
	{
		auto light = lights[i]->GetComponent<Light>();
		light->SetType(LightType::Point);
		light->SetColor(color);
	}
}

void Assignment3Stage::SetScenario2(std::vector<glm::vec3> colors)
{
	int count = (int)colors.size();
	SetActiveLightBalls(count);

	auto lights = OBJECTMANAGER->GetLights();
	for (int i = 0; i < count; ++i)
	{
		auto light = lights[i]->GetComponent<Light>();
		light->SetType(LightType::Spotlight);

		light->SetColor(colors[i]);
	}
}

void Assignment3Stage::SetScenario3(int count)
{
	SetActiveLightBalls(count);

	auto lights = OBJECTMANAGER->GetLights();
	for (int i = 0; i < count; ++i)
	{
		auto light = lights[i]->GetComponent<Light>();
		auto type = rand() % (int)LightType::Count;
		light->SetType(static_cast<LightType>(type));

		float r = (float)rand() / (float)RAND_MAX;
		float g = (float)rand() / (float)RAND_MAX;
		float b = (float)rand() / (float)RAND_MAX;
		light->SetColor({ r,g,b, });
	}
}

void Assignment3Stage::Terminate()
{
}
