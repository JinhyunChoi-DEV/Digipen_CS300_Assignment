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
	void UpdateCamera(float dt);

private:
	float lastUpdateTime;
	SkyBox* skyBox;
};
