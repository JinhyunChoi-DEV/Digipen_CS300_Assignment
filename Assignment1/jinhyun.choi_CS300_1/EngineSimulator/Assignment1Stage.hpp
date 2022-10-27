/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Assignment1Stage.hpp
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


class Assignment1Stage : public StageBase
{
public:
	Assignment1Stage();

	void Initialize() override;
	void Update() override;
	void Terminate() override;

private:
	void LoadAllObjects();
	void UpdateCamera(float dt);
	void UpdateSpheres(float time);

	void CreateCamera();
	void CreateOrbit();
	void CreateObject();
	void CreateSpheres();
	void UpdateGUI();
	std::vector<Object*> spheres;
	glm::vec3 orbitScale{1,1,1};
	glm::vec2 lastMousePos {-1, -1};

	float orbitRadius;
	float pi;
	float lastUpdateTime;
	Camera* camera = nullptr;
	ObjectLoader* objectLoader = nullptr;
	Object* mainObject = nullptr;
	std::string sphereMeshName;
	std::string orbitLineName;
	std::string selectedObject;
	std::vector<std::string> loadFiles;
};
