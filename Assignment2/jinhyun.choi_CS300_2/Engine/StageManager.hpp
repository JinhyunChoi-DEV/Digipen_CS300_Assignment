/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: StageManager.hpp
Purpose: Header to make StageManager
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once
#include <unordered_map>

#include "EngineInterface.hpp"
#include "StageBase.hpp"

class StageManager : public EngineInterface
{
public:
	StageManager();
	StageManager(std::string name, StageBase* stage);

	void Initialize() override;
	void Update() override;
	void Terminate() override;

	void Add(std::string name, StageBase* stage);
	void Change(std::string name);

private:
	StageBase* currentStage;
	std::unordered_map<std::string, StageBase*> stages;

};

extern StageManager* STAGES;