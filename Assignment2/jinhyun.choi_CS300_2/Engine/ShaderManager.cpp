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

void ShaderManager::Compile(std::string name, std::vector<const char*> paths)
{
	int fileSize = paths.size();

	if (paths.size() <= 0)
		return;

	const bool exist = shaders.count(name) > 0;
	if (exist)
	{
		Shader* shader = shaders.find(name)->second;
		glDeleteProgram(shader->ProgramID);
	}

	std::vector<std::pair<ShaderType, std::string>> shaderCodes;
	for(int i = 0; i < fileSize; ++i)
	{
		auto realPath = shaderFilePath + std::string(paths[i]);
		auto shaderInfo = GetShaderCodeWithFile(realPath.c_str());
		shaderCodes.push_back(std::make_pair(shaderInfo.first, shaderInfo.second));
	}

	Shader* shader = new Shader(name, shaderCodes);
	shaders.insert(std::make_pair(name, shader));
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

std::pair<ShaderType, std::string> ShaderManager::GetShaderCodeWithFile(const char* filePath) const
{
	std::ifstream file;
	std::stringstream fileStream;
	std::string firstLine;
	ShaderType type = None;

	file.open(filePath);

	std::getline(file, firstLine);
	if(firstLine.find("#shader") != std::string::npos)
	{
		if(firstLine.find("vertex") != std::string::npos)
			type = Vertex;

		else if(firstLine.find("fragment") != std::string::npos)
			type = Fragment;
		
		else if(firstLine.find("glsl") != std::string::npos)
			type = GLSLCommon;
	}

	fileStream << file.rdbuf();
	file.close();
	
	return std::make_pair(type, fileStream.str());
}