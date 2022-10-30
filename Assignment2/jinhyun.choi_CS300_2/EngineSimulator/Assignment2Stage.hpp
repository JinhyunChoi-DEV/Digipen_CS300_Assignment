/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Assignment2Stage.hpp
Purpose: Header of making Assignment1 Scene
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once
#include <string>
#include <glm/vec3.hpp>

#include "Mesh.hpp"
#include "Camera.hpp"
#include "Object.hpp"
#include "ObjectLoader.hpp"
#include "StageBase.hpp"


class Assignment2Stage : public StageBase
{
public:
	Assignment2Stage();

	void Initialize() override;
	void Update() override;
	void Terminate() override;

private:
	void LoadAllObjects();
	void UpdateCamera(float dt);
	void UpdateLightBall(float time);

	void CreateCamera();
	void CreateOrbit();
	void CreateObject();
	void CreateLightBall();

	void ModelsGUI();
	void NormalDrawGUI();
	void ReloadShaderGUI();
	void LightingBallGUI();
	void SetActiveLightBalls(int count);

	void SetScenario1(int count, glm::vec3 color);
	void SetScenario2(std::vector<glm::vec3> colors);
	void SetScenario3(int count);

	void UpdateGUI();
	std::vector<Object*> lightBalls;
	std::vector<std::string> reloadingShaderNames;
	glm::vec3 orbitScale{1,1,1};
	glm::vec2 lastMousePos {-1, -1};

	int activeLightCount;
	float rotationTime;
	float orbitRadius;
	float pi;
	float lastUpdateTime;
	bool drawVertex;
	bool drawFace;
	bool pauseRotation;
	Camera* camera = nullptr;
	ObjectLoader* objectLoader = nullptr;
	Object* mainObject = nullptr;
	Object* floorObject = nullptr;
	std::string sphereMeshName;
	std::string orbitLineName;
	std::string selectedObject;
	std::string selectedShader;
	std::string selectedLight;
	std::vector<std::string> loadFiles;
};
