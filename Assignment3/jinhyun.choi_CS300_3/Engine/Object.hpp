/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Object.hpp
Purpose: Header to make Object class
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once
#include <typeinfo>
#include <string>
#include <vector>
#include "Component.hpp"

class Object 
{
public:
	Object(std::string name) : name(name)
	{}

	void Update();
	void AddComponent(Component* component);
	void DeleteComponent(Component* component);
	void SetActive(bool active);

	bool IsActive() { return isActive; }
	std::string GetName() { return name; }

	template <typename  COMPONENT>
	COMPONENT* GetComponent();

private:
	std::string name;
	bool isActive = true;
	std::vector<Component*> components;
};

template <typename COMPONENT>
COMPONENT* Object::GetComponent()
{
	for(auto component : components)
	{
		if (typeid(COMPONENT) == typeid(*component))
			return dynamic_cast<COMPONENT*>(component);
	}

	return nullptr;
}
