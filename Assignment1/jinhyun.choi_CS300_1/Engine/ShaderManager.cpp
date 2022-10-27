/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ShaderManager.cpp
Purpose: Making of manager about all shader for use compile, load, and caching
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include "ShaderManager.hpp"
#include "Shader.hpp"


ShaderManager::ShaderManager()
{
}

void ShaderManager::Compile(const char* vertexFile, const char* fragFile, std::string name)
{
	const bool existVertexFile = strlen(vertexFile) > 0;
	const bool existFragFile = strlen(fragFile) > 0;
	if(!existVertexFile || !existFragFile)
	{
		std::cout << "Error-Shader: Compile Error with vertexPath or fragmentPath" << std::endl;
		return;
	}

	const bool exist = shaders.count(name) > 0;
	if(exist)
	{
		Shader* shader = shaders.find(name)->second;
		glDeleteProgram(shader->ProgramID);
	}

	auto vertexPath = std::string(shaderFilePath) + std::string(vertexFile);
	auto fragPath = std::string(shaderFilePath) + std::string(fragFile);
	auto vertexString = GetShaderCodeWithFile(vertexPath.c_str());
	auto fragmentString = GetShaderCodeWithFile(fragPath.c_str());

	Shader* shader = new Shader(vertexString, fragmentString, name);
	shaders.insert(std::make_pair(shader->Name, shader));
}

void ShaderManager::Use(std::string name)
{
	const bool exist = shaders.count(name) > 0;
	if(!exist)
	{
		std::cout << "Error-Shader: Try To Use Unavailable Shader: "<< name << std::endl;
		return;
	}

	const auto shader = shaders[name];
	glUseProgram(shader->ProgramID);
}

Shader* ShaderManager::GetShader(std::string name)
{
	const bool exist = shaders.count(name) > 0;
	if (shaders.empty() || !exist)
		return nullptr;

	return shaders[name];
}

std::string ShaderManager::GetShaderCodeWithFile(const char* filePath) const
{
	std::ifstream file;
	std::stringstream fileStream;

	file.open(filePath);
	fileStream << file.rdbuf();
	file.close();

	return fileStream.str();
}
