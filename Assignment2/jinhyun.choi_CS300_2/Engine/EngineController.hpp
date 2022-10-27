/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: EngineController.hpp
Purpose: Header of controller about Engine cores
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once
#include <vector>
#include "EngineInterface.hpp"
#include "Input.hpp"
#include "MeshManager.hpp"


class EngineController
{
public:
	EngineController();
	~EngineController();

	void Initialize();
	void Update();
	void Terminate();

private:
	InputManager* inputManager;
	MeshManager* meshManager;
	std::vector<EngineInterface*> engineCores;
	bool isInitialize;
	bool isRunning;
};
