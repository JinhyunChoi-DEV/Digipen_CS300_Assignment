/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Input.cpp
Purpose: Making input system with GLFW callback function
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include <iostream>

#include "Input.hpp"

InputManager* INPUT;

InputManager::InputManager()
{
	INPUT = this;
	clear();
}

void InputManager::Initialize()
{
	clear();
}

void InputManager::Update()
{
	keyTriggered.reset();
	mouseButtonTriggered.reset();
}

void InputManager::Terminate()
{
	clear();
}

void InputManager::SetKeyboard(int key, int action)
{
	KeyActionState state = convert(action);

	switch (state)
	{
	case KeyActionState::Release:
		keyPressed.reset(key);
		keyTriggered.reset(key);
		break;

	case KeyActionState::Press:
		keyPressed.set(key);
		keyTriggered.set(key);
		break;

	case KeyActionState::Repeat:
		keyPressed.set(key);
		keyTriggered.reset(key);
		break;

	case KeyActionState::None:
		fprintf(stderr, "KeyActionState Error");
		break;
	}
}

void InputManager::SetMouse(double x, double y)
{
	mousePosition.x = (float)x;
	mousePosition.y = (float)y;
}

void InputManager::SetMouseButton(int key, int action)
{
	KeyActionState state = convert(action);

	switch (state)
	{
	case KeyActionState::Release:
		mouseButtonPressed.reset(key);
		mouseButtonTriggered.reset(key);
		break;

	case KeyActionState::Press:
		mouseButtonPressed.set(key);
		mouseButtonTriggered.set(key);
		break;

	case KeyActionState::Repeat:
		mouseButtonPressed.set(key);
		mouseButtonTriggered.reset(key);
		break;

	case KeyActionState::None:
		fprintf(stderr, "KeyActionState Error");
		break;
	}
}

bool InputManager::IsPressed(int key)
{
	return keyPressed[key];
}

bool InputManager::IsReleased(int key)
{
	return !keyPressed[key];
}

bool InputManager::IsTriggered(int key)
{
	return keyTriggered[key];
}

bool InputManager::IsPressedMouseButton(int key)
{
	return mouseButtonPressed[key];
}

bool InputManager::IsReleasedMouseButton(int key)
{
	return !mouseButtonPressed[key];
}

bool InputManager::IsTriggeredMouseButton(int key)
{
	return mouseButtonTriggered[key];
}

void InputManager::clear()
{
	keyPressed.reset();
	keyTriggered.reset();
	mouseButtonPressed.reset();
	mouseButtonTriggered.reset();
}

KeyActionState InputManager::convert(int key)
{
	if (key == GLFW_RELEASE)
		return KeyActionState::Release;

	if (key == GLFW_PRESS)
		return KeyActionState::Press;

	if (key == GLFW_REPEAT)
		return KeyActionState::Repeat;

	return KeyActionState::None;
}
