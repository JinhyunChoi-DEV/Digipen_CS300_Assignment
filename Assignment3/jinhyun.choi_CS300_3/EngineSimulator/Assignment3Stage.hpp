#pragma once

#include <glm/vec3.hpp>

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
