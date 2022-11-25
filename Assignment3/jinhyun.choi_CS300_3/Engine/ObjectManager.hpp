/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ObjectManager.hpp
Purpose: Header to make ObjectManager class
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once
#include <unordered_map>
#include "EngineInterface.hpp"
class Object;

class ObjectManager : public EngineInterface
{
public:
	ObjectManager();

	void Initialize() override;
	void Update() override;
	void Terminate() override;

	void Add(std::string name);
	void Add(std::string name, Object* obj);
	void Delete(std::string name);

	std::vector<Object*> GetLights() const;
	std::vector<Object*> GetEnvironmentObjects() const;
	std::unordered_map<std::string, Object*>& GetObjects() const {return objects;}
	Object* GetObject(std::string name) const { return objects[name]; }

private:
	bool IsExist(std::string name);
	mutable std::unordered_map<std::string, Object*> objects;
};

extern ObjectManager* OBJECTMANAGER;