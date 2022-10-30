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
	void CreateLightObject();
	void CreateObject();
	void CreateLightBall();
	void ModelsGUI();
	void NormalDrawGUI();
	void ReloadShaderGUI();

	void UpdateGUI();
	std::vector<Object*> spheres;
	std::vector<std::string> reloadingShaderNames;
	glm::vec3 orbitScale{1,1,1};
	glm::vec2 lastMousePos {-1, -1};

	float orbitRadius;
	float pi;
	float lastUpdateTime;
	bool drawVertex;
	bool drawFace;
	Camera* camera = nullptr;
	ObjectLoader* objectLoader = nullptr;
	Object* mainObject = nullptr;
	Object* floorObject = nullptr;
	std::string sphereMeshName;
	std::string orbitLineName;
	std::string selectedObject;
	std::string selectedShader;
	std::vector<std::string> loadFiles;
};
