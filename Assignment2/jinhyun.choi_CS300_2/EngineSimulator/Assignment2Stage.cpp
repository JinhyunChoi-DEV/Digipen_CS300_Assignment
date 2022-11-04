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
#include "Light.hpp"
#include "LightTypeHelper.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "TextureTypeHelper.hpp"

Assignment2Stage::Assignment2Stage()
{
	sphereMeshName = "Default_Sphere";
	orbitLineName = "Line_Orbit";
	objectLoader = new ObjectLoader();
	ppmFileReader = new PPMFileReader();
	orbitRadius = 1.0f;
	lastUpdateTime = 0.0f;
	rotationTime = 0.0f;
	drawVertex = false;
	drawFace = false;
	pauseRotation = false;
	activeLightCount = 1;
	visualizeUV = false;
	pi = glm::pi<float>();

	ppmFileReader->Read("metal_roof_diff_512x512.ppm");
	ppmFileReader->Read("metal_roof_spec_512x512.ppm");
	ppmFileReader->Read("grid.ppm");
	GRAPHIC->CompileShader("Solid", "Solid.vert", "Solid.frag", "TransformModel.glsl", nullptr);
	GRAPHIC->CompileShader("PhongShading", "PhongShading.vert", "PhongShading.frag", "Texture.glsl", "Light.glsl", "TransformModel.glsl", nullptr);
	//GRAPHIC->CompileShader("PhongLighting", "PhongLighting.vert", "PhongLighting.frag", "Texture.glsl", "Light.glsl", "TransformModel.glsl", nullptr);
	//GRAPHIC->CompileShader("BlinnShading", "BlinnShading.vert", "BlinnShading.frag", "Texture.glsl", "Light.glsl", "TransformModel.glsl", nullptr);
	GRAPHIC->CompileShader("Line", "Line.vert", "Line.frag", "TransformModel.glsl", nullptr);

	reloadingShaderNames.insert(reloadingShaderNames.begin(), { "PhongShading", "PhongLighting" , "BlinnShading"});
}

void Assignment2Stage::Initialize()
{
	CreateCamera();
	LoadAllObjects();
	CreateObject();
	CreateOrbit();
	CreateLightBall();

	selectedShader = reloadingShaderNames[0];
}

void Assignment2Stage::Update()
{
	float time = (float)glfwGetTime();
	float dt = time - lastUpdateTime;

	UpdateCamera(dt);

	if(!pauseRotation)
		UpdateLightBall(dt);

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

void Assignment2Stage::UpdateLightBall(float dt)
{
	rotationTime += dt;
	auto lights = OBJECTMANAGER->GetLights();
	float step = 2 * pi / lights.size();
	auto scale = orbitScale;

	for (unsigned i = 0; i < lights.size(); ++i)
	{
		float additionAngle = step * i;
		auto transform = lights[i]->GetComponent<Transform>();
		float x = scale.x * cosf(rotationTime + additionAngle);
		float y = 0;
		float z = scale.y * sinf(rotationTime + additionAngle);
		transform->SetTranslate({ x, y , z });

		auto light = lights[i]->GetComponent<Light>();
		auto mainTransform = mainObject->GetComponent<Transform>();

		auto newDir = mainTransform->GetPosition() - transform->GetPosition();
		light->SetDirection(newDir);
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

void Assignment2Stage::CreateOrbit()
{
	auto orbitMesh = OrbitSphere(orbitLineName, orbitRadius, 50);
	auto meshOrbit = orbitMesh.GetMesh();
	MESHES->Add(meshOrbit->GetName(), meshOrbit);

	Object* orbit = new Object("orbit");
	orbit->AddComponent(meshOrbit);
	orbit->AddComponent(new Transform());
	orbit->GetComponent<Transform>()->SetTranslate(glm::vec3{ 0, 0, 0 });
	orbit->GetComponent<Transform>()->SetScale(glm::vec3{ 5, 5, 5});
	orbitScale = orbit->GetComponent<Transform>()->GetScale();
	OBJECTMANAGER->Add("orbit", orbit);
}

void Assignment2Stage::CreateObject()
{
	auto texture = new Texture();
	texture->SetMappingType(TextureMappingType::Cube);
	texture->SetDiffuse("metal_roof_diff_512x512.ppm");
	texture->SetSpecular("metal_roof_spec_512x512.ppm");
	texture->SetAmbientColor({ 0,0,0 });
	texture->SetDiffuseColor({ 1,1,1 });
	texture->SetSpecularColor({ 1,1,1 });

	auto main = new Object("MainObject");
	main->AddComponent(new Mesh(MESHES->GetMesh(loadFiles[0])));
	main->AddComponent(new Transform);
	main->AddComponent(texture);
	OBJECTMANAGER->Add("MainObject", main);
	mainObject = main;
	selectedObject = loadFiles[0];
	mainObject->GetComponent<Transform>()->SetTranslate(glm::vec3(0));
	mainObject->GetComponent<Transform>()->SetScale(glm::vec3(1.5));

	auto floor = new Object("FloorObject");
	floor->AddComponent(new Mesh(MESHES->GetMesh("quad")));
	floor->AddComponent(new Transform);
	floor->GetComponent<Mesh>()->SetColor(glm::vec3(0.5, 0.5, 0));
	OBJECTMANAGER->Add("FloorObject", floor);
	floorObject = floor;
	floorObject->GetComponent<Transform>()->SetTranslate(glm::vec3(0, -1.5, 0));
	floorObject->GetComponent<Transform>()->SetScale(glm::vec3(10));
	floorObject->GetComponent<Transform>()->SetRotation(-90, X);
}

void Assignment2Stage::CreateLightBall()
{
	int objectCount = 16;
	glm::vec3 scale = orbitScale;
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
		float x = orbitRadius * cosf(angle);
		float y = 0.0f;
		float z = orbitRadius * sinf(angle);

		std::string name = baseName + std::to_string(i + 1);
		Object* object = new Object(name);
		object->AddComponent(meshSphere);
		object->AddComponent(new Light());

		if (activeLightCount <= i)
			object->SetActive(false);

		object->AddComponent(new Transform());
		object->GetComponent<Transform>()->SetTranslate({ x*scale.x, y*scale.y, z* scale.z});
		object->GetComponent<Transform>()->SetScale(glm::vec3{ 0.2, 0.2, 0.2 });
		OBJECTMANAGER->Add(name, object);

		if (i == 0)
			selectedLight = name;

		auto light = object->GetComponent<Light>();
		light->SetType(LightType::Point);
		light->SetDirection({1,0,0});
		light->SetInnerAngle(23.0f);
		light->SetOuterAngle(27.5f);
		lightBalls.push_back(object);
	}
}

void Assignment2Stage::UpdateGUI()
{
	ModelsGUI();
	NormalDrawGUI();
	ReloadShaderGUI();
	LightingBallGUI();
	MaterialGUI();
	GlobalLightGUI();
}

void Assignment2Stage::ModelsGUI()
{
	if(ImGui::CollapsingHeader("Models"))
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
	}
}

void Assignment2Stage::NormalDrawGUI()
{
	if (ImGui::CollapsingHeader("DrawNormal"))
	{
		ImGui::Checkbox("Draw Vertex Normal", &drawVertex);
		ImGui::Checkbox("Draw Face Normal", &drawFace);

		GRAPHIC->SetDrawNormal(drawVertex, drawFace);
	}
}

void Assignment2Stage::ReloadShaderGUI()
{
	if (ImGui::CollapsingHeader("Shader"))
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
	}
}

void Assignment2Stage::LightingBallGUI()
{
	auto selectedLightObject = OBJECTMANAGER->GetObject(selectedLight);
	auto lights = OBJECTMANAGER->GetLights();
	std::sort(lights.begin(), lights.end(), [](Object* a, Object* b)
	{
		if(a->GetName().size() == b->GetName().size())
			return a->GetName() < b->GetName();

		return a->GetName().size() < b->GetName().size();
	});

	if(ImGui::CollapsingHeader("Light Control"))
	{
		ImGui::Text("Light Object Count");
		if(ImGui::SliderInt("Count", &activeLightCount, 1, 16))
			SetActiveLightBalls(activeLightCount);

		ImGui::NewLine();
		ImGui::Text("Orbit Rotate Action");
		ImGui::Checkbox("Pause Rotation", &pauseRotation);

		// Scenarios Buttons
		ImGui::NewLine();
		ImGui::Text("Lighting Scenarios");
		if(ImGui::Button("Scenario 1"))
		{
			glm::vec3 color = { 1,1,1 };
			SetScenario1(5, color);
		}
		ImGui::SameLine();

		if(ImGui::Button("Scenario 2"))
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

		if(ImGui::Button("Scenario 3"))
		{
			SetScenario3(11);
		}
		ImGui::SameLine();

		// All Light Lists
		ImGui::NewLine();
		ImGui::Text("Light List");
		if(ImGui::BeginCombo("Selected Light", selectedLight.c_str()))
		{
			for(int i =0; i<lights.size(); ++i)
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
		if(ImGui::TreeNode("Light Status"))
		{
			auto light = selectedLightObject->GetComponent<Light>();
			auto type = light->GetType();
			auto preview = GetLightTypeString(type);
			if(ImGui::BeginCombo("Type", preview.c_str()))
			{
				auto lightTypes = GetLightTypesString();
				for(int i =0; i<lightTypes.size(); ++i)
				{
					bool isSelected = preview == lightTypes[i];
					if(ImGui::Selectable(lightTypes[i].c_str(), isSelected))
					{
						type = GetTypeFromString(lightTypes[i]);
						light->SetType(type);
					}

					if(isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			// Color
			auto color = light->GetAmientIntensity();
			if(ImGui::ColorEdit3("Ambient", &color[0]))
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
			if(type == LightType::Spotlight)
			{
				auto innerAngle = light->GetInnerAngle();
				auto outerAngle = light->GetOuterAngle();
				auto fallout = light->GetFallOut();

				if(ImGui::SliderFloat("Inner Angle", &innerAngle, 0, 90))
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
}

void Assignment2Stage::SetActiveLightBalls(int count)
{
	activeLightCount = count;
	for (int i = 0; i < lightBalls.size(); ++i)
	{
		if (activeLightCount <= i)
			lightBalls[i]->SetActive(false);
		else
			lightBalls[i]->SetActive(true);
	}
	selectedLight = lightBalls[0]->GetName();
}

void Assignment2Stage::MaterialGUI()
{
	if (ImGui::CollapsingHeader("Material Control"))
	{
		auto texture = mainObject->GetComponent<Texture>();
		auto entityType = texture->GetEntityType();
		auto mappingType = texture->GetMappingType();
		auto isGPU = texture->IsGPUMode();

		// Color
		auto color = texture->GetAmbientColor();
		if (ImGui::ColorEdit3("Ambient", &color[0]))
		{
			texture->SetAmbientColor(color);
		}

		color = texture->GetDiffuseColor();
		if (ImGui::ColorEdit3("Diffuse", &color[0]))
		{
			texture->SetDiffuseColor(color);
		}

		color = texture->GetSpecularColor();
		if (ImGui::ColorEdit3("Specular", &color[0]))
		{
			texture->SetSpecularColor(color);
		}

		color = texture->GetEmissiveColor();
		if (ImGui::ColorEdit3("Emissive", &color[0]))
		{
			texture->SetEmissiveColor(color);
		}

		auto mappingPreview = GetTextureMappingTypeString(mappingType);
		if(ImGui::BeginCombo("Texture Mode", mappingPreview.c_str()))
		{
			auto types = GetTextureMappingTypesString();
			for(int i =0; i<types.size(); ++i)
			{
				bool isSelected = mappingPreview == types[i];
				if(ImGui::Selectable(types[i].c_str(), isSelected))
				{
					auto type = GetTextureMappingFromString(types[i]);
					texture->SetMappingType(type);
					GRAPHIC->RebindTexture(mainObject);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		if(ImGui::Checkbox("Visualize UV", &visualizeUV))
		{
			if(visualizeUV)
			{
				texture->SetDiffuse("grid.ppm");
				texture->SetSpecular("grid.ppm");
			}
			else
			{
				texture->SetDiffuse("metal_roof_diff_512x512.ppm");
				texture->SetSpecular("metal_roof_spec_512x512.ppm");
			}
		}

		if (ImGui::Checkbox("GPU Mode", &isGPU))
		{
			texture->SetUseGPU(isGPU);
		}

		auto entityPreview = GetTextureEntityTypeString(entityType);
		if (ImGui::BeginCombo("Texture Entity", entityPreview.c_str()))
		{
			auto types = GetTextureEntityTypesString();
			for (int i = 0; i < types.size(); ++i)
			{
				bool isSelected = entityPreview == types[i];
				if (ImGui::Selectable(types[i].c_str(), isSelected))
				{
					auto type = GetTextureEntityTypeFromString(types[i]);
					texture->SetEntityType(type);
					GRAPHIC->RebindTexture(mainObject);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
	}
}

void Assignment2Stage::GlobalLightGUI()
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

void Assignment2Stage::SetScenario1(int count, glm::vec3 color)
{
	SetActiveLightBalls(count);

	auto lights = OBJECTMANAGER->GetLights();
	for(int i =0; i<count; ++i)
	{
		auto light = lights[i]->GetComponent<Light>();
		light->SetType(LightType::Point);
		light->SetColor(color);
	}
}

void Assignment2Stage::SetScenario2(std::vector<glm::vec3> colors)
{
	int count = colors.size();
	SetActiveLightBalls(count);

	auto lights = OBJECTMANAGER->GetLights();
	for (int i = 0; i < count; ++i)
	{
		auto light = lights[i]->GetComponent<Light>();
		light->SetType(LightType::Spotlight);

		light->SetColor(colors[i]);
	}
}

void Assignment2Stage::SetScenario3(int count)
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
