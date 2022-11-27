/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: TextureManager.hpp
Purpose: the header file for reading TextureManager
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

#pragma once
#include <string>
#include <unordered_map>
#include "Image.hpp"

class TextureManager
{
public:
	TextureManager();

	bool IsExist(std::string file);
	void Read(std::string fileName);
	Image Get(std::string file);

private:
	std::unordered_map<std::string, Image> textures;
	std::string textureFilePath = "../Textures/";
};

extern TextureManager* TEXTURES;