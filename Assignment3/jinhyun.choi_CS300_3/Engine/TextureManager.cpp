/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: TextureManager.cpp
Purpose: the cpp file for reading PPMFiles
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/04/2022
End Header --------------------------------------------------------*/
#include "TextureManager.hpp"

#include <stb_image.h>

TextureManager* TEXTURES = nullptr;

TextureManager::TextureManager()
{
	stbi_set_flip_vertically_on_load(true);

	if(TEXTURES ==	nullptr)
		TEXTURES = this;
}

bool TextureManager::IsExist(std::string file)
{
	return textures.find(file) != textures.end();
}

void TextureManager::Read(std::string fileName)
{
	if (IsExist(fileName))
		return;

	std::string path = textureFilePath + fileName;
	Image image;
	image.Read(path);

	textures.insert(std::make_pair(fileName, image));
}

Image TextureManager::Get(std::string file)
{
	return textures[file];
}
