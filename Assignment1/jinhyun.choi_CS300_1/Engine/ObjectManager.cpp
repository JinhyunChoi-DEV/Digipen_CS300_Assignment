/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ObjectManager.cpp
Purpose: Making manager of objects for control all objects in current scene.
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include <iostream>

#include "ObjectManager.hpp"
#include "Object.hpp"

ObjectManager* OBJECTMANAGER;

ObjectManager::ObjectManager()
{
	OBJECTMANAGER = this;
}

void ObjectManager::Initialize()
{
	objects.clear();
}

void ObjectManager::Update()
{
	for(auto pair : objects)
	{
		auto object = pair.second;
		object->Update();
	}
}

void ObjectManager::Terminate()
{
	for (auto pair : objects)
	{
		auto object = pair.second;
		delete object;
	}

	objects.clear();
}

void ObjectManager::Add(std::string name)
{
	if (IsExist(name))
	{
		std::cout << "Error-ObjectManager: Already Added Object: " << name << std::endl;
		return;
	}

	objects.insert(std::make_pair(name, new Object()));
}

void ObjectManager::Add(std::string name, Object* obj)
{
	if (IsExist(name))
	{
		std::cout << "Error-ObjectManager: Already Added Object: " << name << std::endl;
		return;
	}

	objects.insert(std::make_pair(name, obj));
}

void ObjectManager::Delete(std::string name)
{
	if (!IsExist(name))
	{
		std::cout << "Error-ObjectManager: Try Delete Not Exist Object: " << name << std::endl;
		return;
	}

	auto object = objects.find(name)->second;
	delete object;

	objects.erase(name);
}

bool ObjectManager::IsExist(std::string name)
{
	return objects.count(name) > 0;
}

