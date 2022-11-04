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

enum ShaderType
{
	Vertex, Fragment, GLSLCommon, None
};

class Shader;
class ShaderManager
{
public:
	ShaderManager();

	void Compile(std::string name, std::vector<const char*> paths);
	void Use(std::string name);

	Shader* GetShader(std::string name);
	std::unordered_map<std::string, Shader*> GetShaders() const { return shaders; }

private:
	std::pair<ShaderType, std::string> GetShaderCodeWithFile(const char* filePath) const;
	std::string commonFile = "LightingFunctions.glsl";
	std::string shaderFilePath = "../ShaderFiles/";
	std::unordered_map<std::string, Shader*> shaders;

};
