/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Input.hpp
Purpose: Header of Input class
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once
#include <bitset>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

enum class KeyActionState
{
	None,
	Release,
	Press,
	Repeat
};

class InputManager
{
public:
	InputManager();
	void Initialize();
	void Update();
	void Terminate();
	void SetKeyboard(int key, int action);
	void SetMouse(double x, double y);
	void SetMouseButton(int key, int action);

	bool IsPressed(int key);
	bool IsReleased(int key);
	bool IsTriggered(int key);

	bool IsPressedMouseButton(int key);
	bool IsReleasedMouseButton(int key);
	bool IsTriggeredMouseButton(int key);

	glm::vec2 GetMousePosition() { return mousePosition; }
private:
	void clear();
	KeyActionState convert(int key);
	glm::vec2 mousePosition{ 0,0 };

	std::bitset<GLFW_KEY_MENU> keyPressed;
	std::bitset<GLFW_KEY_MENU> keyTriggered;

	std::bitset<GLFW_MOUSE_BUTTON_LAST> mouseButtonPressed;
	std::bitset<GLFW_MOUSE_BUTTON_LAST> mouseButtonTriggered;
};

extern InputManager* INPUT;