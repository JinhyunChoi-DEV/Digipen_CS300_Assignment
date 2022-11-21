/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: EngineController.cpp
Purpose: Making of controller about engine cores
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include <iostream>
#include "EngineController.hpp"
#include "Application.hpp"
#include "Graphic.hpp"
#include "Object.hpp"
#include "ObjectManager.hpp"
#include "StageManager.hpp"

EngineController::EngineController()
{
	isRunning = false;
	isInitialize = false;

	inputManager = new InputManager();
	meshManager = new MeshManager();
	textureManager = new TextureManager();
	objLoader = new ObjectLoader();
	engineCores.push_back(new Application());
	engineCores.push_back(new Graphic());
	engineCores.push_back(new ObjectManager());
	engineCores.push_back(new StageManager());
}

EngineController::~EngineController()
{
	delete inputManager;
	delete meshManager;
	engineCores.clear();
}

void EngineController::Initialize()
{
	inputManager->Initialize();
	meshManager->Initialize();

	for (auto core : engineCores)
		core->Initialize();

	isRunning = true;
	isInitialize = true;
}

void EngineController::Update()
{
	while (!APPLICATION->IsWindowClose() && isRunning)
	{
		inputManager->Update();
		GUI->Begin();

		for (auto core : engineCores)
			core->Update();

		GUI->End();

		if (INPUT->IsTriggered(GLFW_KEY_ESCAPE))
			isRunning = false;
	}
}

void EngineController::Terminate()
{
	for (auto core : engineCores)
	{
		core->Terminate();
	}

	isRunning = false;
}
