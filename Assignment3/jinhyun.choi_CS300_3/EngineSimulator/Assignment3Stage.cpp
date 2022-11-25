#include <filesystem>

#include "Assignment3Stage.hpp"
#include "Application.hpp"
#include "Graphic.hpp"
#include "imgui.h"
#include "Input.hpp"
#include "Light.hpp"
#include "LightTypeHelper.hpp"
#include "MeshManager.hpp"
#include "ObjectLoader.hpp"
#include "ObjectManager.hpp"
#include "Object.hpp"
#include "SphereMesh.hpp"

Assignment3Stage::Assignment3Stage()
{
	pauseRotation = false;
	sphereMeshName = "Default_Sphere";
	pi = glm::pi<float>();
	rotationTime = 0.0f;
	activeLightCount = 1;
	drawFace = false;
	drawVertex = false;

	GRAPHIC->CompileShader("FrameBuffer", "FrameBuffer.vert", "FrameBuffer.frag", "TransformModel.glsl", nullptr);
	GRAPHIC->CompileShader("EnvironmentMapping", "EnvironmentMapping.vert", "EnvironmentMapping.frag", "TransformModel.glsl", nullptr);
	GRAPHIC->CompileShader("Solid", "Solid.vert", "Solid.frag", "TransformModel.glsl", nullptr);
	GRAPHIC->CompileShader("Skybox", "Skybox.vert", "Skybox.frag", "TransformModel.glsl", nullptr);
	GRAPHIC->CompileShader("Line", "Line.vert", "Line.frag", "TransformModel.glsl", nullptr);
}

void Assignment3Stage::Initialize()
{
	skyBox = new SkyBox();
	GRAPHIC->SetSkyBox(skyBox);

	LoadAllObjects();
	CreateMainObject();
	CreateLightBall();

	auto camera = GRAPHIC->GetCamera();
	CameraOriginData data;
	data.position = { 0,0,20 };
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

void Assignment3Stage::NormalDrawGUI()
{
	if (ImGui::CollapsingHeader("DrawNormal"))
	{
		ImGui::Checkbox("Draw Vertex Normal", &drawVertex);
		ImGui::Checkbox("Draw Face Normal", &drawFace);

		GRAPHIC->SetDrawNormal(drawVertex, drawFace);
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
	auto main = new Object("MainObject");
	main->AddComponent(new Mesh(MESHES->GetMesh("sphere")));
	main->AddComponent(new Transform);
	OBJECTMANAGER->Add("MainObject", main);
	mainObject = main;
	selectedMesh = "sphere";
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
	NormalDrawGUI();
	LightingBallGUI();
}

void Assignment3Stage::LightingBallGUI()
{
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

		ImGui::NewLine();
		ImGui::Text("Orbit Rotate Action");
		ImGui::Checkbox("Pause Rotation", &pauseRotation);

		// Scenarios Buttons
		ImGui::NewLine();
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
