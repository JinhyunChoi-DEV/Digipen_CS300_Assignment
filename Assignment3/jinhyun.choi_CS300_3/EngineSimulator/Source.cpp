/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Source.cpp
Purpose: Generate Core Engine and do starting, updating, and terminate application
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include <cstdarg>
#include <iostream>

#include "Assignment2Stage.hpp"
#include "EngineController.hpp"
#include "StageManager.hpp"
#include "Assignment3Stage.hpp"

int main()
{
	EngineController* engine = new EngineController();
	STAGES->Add("Assignment3", new Assignment3Stage());

	engine->Initialize();

	engine->Update();

	engine->Terminate();

	delete engine;

	return 0;
}
