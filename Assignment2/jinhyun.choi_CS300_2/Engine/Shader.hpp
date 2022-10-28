/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Shader.hpp
Purpose: Header of Shader class
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "ShaderManager.hpp"

class Shader
{
public:
	Shader(std::string name, std::vector<std::pair<ShaderType, std::string>> shaderInfos);

	unsigned int ProgramID = 0;
	std::string Name;

	void Use();
	void Set(const std::string& name, int value) const;
	void Set(const std::string& name, bool value) const;
	void Set(const std::string& name, float value) const;
	void Set(const std::string& name, glm::vec3 value) const;
	void Set(const std::string& name, glm::vec4 value) const;
	void Set(const std::string& name, glm::mat4 value) const;

private:
	std::string versionText = "#version 400 core\n";
};
