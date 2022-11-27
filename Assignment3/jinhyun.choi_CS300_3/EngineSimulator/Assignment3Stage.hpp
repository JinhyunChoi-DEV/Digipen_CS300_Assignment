/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Assignment3Stage.hpp
Purpose: the header file for Assignment3Stage data and function
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/27/2022
End Header --------------------------------------------------------*/

#pragma once

#include <glm/glm.hpp>

#include "StageBase.hpp"
#include "Object.hpp"

class SkyBox;

class Assignment3Stage : public StageBase
{
public:
	Assignment3Stage();

	void Initialize();
	void Update();
	void Terminate();

private:
	void UpdateCamera(float dt);
	void UpdateLightBall(float time);
	void LoadAllObjects();
	void CreateMainObject();
	void CreateLightBall();

	void UpdateGUI();
	void ModelsGUI();
	void NormalDrawGUI();
	void LightingBallGUI();
	void MaterialGUI();
	void GlobalLightGUI();

	void SetActiveLightBalls(int count);
	void SetScenario1(int count, glm::vec3 color);
	void SetScenario2(std::vector<glm::vec3> colors);
	void SetScenario3(int count);

	Object* mainObject = nullptr;
	std::vector<Object*> lightBalls;

	float pi;
	float rotationTime;
	bool pauseRotation;
	bool drawVertex;
	bool drawFace;
	int activeLightCount;
	float lastUpdateTime;
	std::string selectedLight;
	std::string sphereMeshName;
	SkyBox* skyBox;
	std::string selectedMesh;
	std::string selectedObject;
	std::string selectedMaterials;
	int materialsIndex;
	std::vector<std::string> loadFiles;
	std::vector<std::pair<std::string, float>> refractionFactor;
};
