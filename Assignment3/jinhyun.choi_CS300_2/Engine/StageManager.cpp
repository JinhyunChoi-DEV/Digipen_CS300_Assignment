/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: StageManager.cpp
Purpose: Make Manager of stage for control current and totals stages
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include <iostream>

#include "StageManager.hpp"

StageManager* STAGES;
StageManager::StageManager()
	: currentStage(nullptr)
{
	STAGES = this;
}

StageManager::StageManager(std::string name, StageBase* stage)
{
	STAGES = this;

	currentStage = stage;
	Add(name, stage);
}

void StageManager::Initialize()
{
	if (currentStage == nullptr)
		currentStage = stages.begin()->second;

	currentStage->Initialize();
}

void StageManager::Update()
{
	if (currentStage == nullptr)
	{
		std::cout << "Error-StageManager: Current Stage Is Null" << std::endl;
		return;
	}
	currentStage->Update();
}

void StageManager::Terminate()
{
	currentStage->Terminate();
	currentStage = nullptr;
}

void StageManager::Add(std::string name, StageBase* stage)
{
	stages[name] = stage;
}

void StageManager::Change(std::string name)
{
	currentStage->Terminate();
	currentStage = stages[name];
	currentStage->Initialize();
}
