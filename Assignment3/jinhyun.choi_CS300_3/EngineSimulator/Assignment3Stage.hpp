#pragma once
#include "Skybox.hpp"
#include "StageBase.hpp"

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
	void NormalDrawGUI();
	void LoadAllObjects();
	void CreateMainObject();
	void CreateLightBall();
	void LightingBallGUI();

	void SetActiveLightBalls(int count);
	void SetScenario1(int count, glm::vec3 color);
	void SetScenario2(std::vector<glm::vec3> colors);
	void SetScenario3(int count);
	void UpdateGUI();

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
	std::vector<std::string> loadFiles;
};
