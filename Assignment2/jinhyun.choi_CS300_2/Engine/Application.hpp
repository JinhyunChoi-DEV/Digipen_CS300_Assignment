/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Application.hpp
Purpose: Header of application class
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "EngineInterface.hpp"
#include "GUIManager.hpp"

class Application : public EngineInterface
{
public:
	Application();
	~Application();
	void Initialize() override;
	void Update() override;
	void Terminate() override;

	bool IsWindowClose() const;
	double GetGameTime() const;
	void SetWindowSize(int width, int height);
	glm::vec2 GetWindowSize() const { return windowSize; }

private:
	GLFWwindow* window = nullptr;
	GUIManager* gui = nullptr;
	glm::vec2 windowSize{};
};

extern Application* APPLICATION;