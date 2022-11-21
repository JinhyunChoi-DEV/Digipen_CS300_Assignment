/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Image.hpp
Purpose: the header file for saving Image file data
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

#pragma once
#include <string>

class Image
{
public:
	Image() = default;

	void Read(std::string path);

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	int GetChannel() const { return channel; }
	unsigned char* GetBuffer() const { return buffer; }
	unsigned int GetBufferObject() const { return bufferObject; }

private:
	std::string path = "";
	int width = 0;
	int height = 0;
	int channel = 0;
	unsigned int bufferObject = -1;
	unsigned char* buffer = nullptr;
};
