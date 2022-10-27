/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ShaderManager.hpp
Purpose: Header of manager about shader
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once
#include <string>
#include <unordered_map>

class Shader;

class ShaderManager
{
public:
	ShaderManager();

	void Compile(const char* vertexPath, const char* fragmentPath, std::string name);
	void Use(std::string name);
	Shader* GetShader(std::string name);

private:
	std::string GetShaderCodeWithFile(const char* filePath) const;
	const char* shaderFilePath = "../Engine/ShaderFiles/";
	std::unordered_map<std::string, Shader*> shaders;

};
