/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Object.cpp
Purpose: Making object core system and manage components.
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include "Object.hpp"
#include "Mesh.hpp"

void Object::Update()
{
	for(auto component : components)
	{
		component->Update();
	}
}

void Object::AddComponent(Component* component)
{
	components.push_back(component);
}

void Object::DeleteComponent(Component* component)
{
	for(auto it = components.begin(); it != components.end();)
	{
		if (*it == component)
			it = components.erase(it);
		else
			++it;
	}
}

void Object::SetActive(bool active)
{
	isActive = active;
}


