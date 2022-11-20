/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: GUIManager.hpp
Purpose: Header of manager about GUI by using Dear ImGui
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once
#include <GLFW/glfw3.h>

class GUIManager
{
public:
	GUIManager(GLFWwindow* window);
	~GUIManager() = default;

	void Initialize();
	void Terminate();
	void Begin();
	void End();

private:
	const char* glslVersion;
	GLFWwindow* window = nullptr;
};

extern GUIManager* GUI;