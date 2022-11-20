/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: PPM.hpp
Purpose: the header file for saving PPM file data
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

#pragma once
#include <string>

class PPM
{
public:
	PPM() = default;

	void Read(std::string path);

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	int GetMax() const { return maxValue; }
	float* GetBuffer() const { return buffer; }
	unsigned int GetBufferObject() const { return bufferObject; }

private:
	std::string mMagic, path = "";
	int width = 0;
	int height = 0;
	int maxValue = -1;
	unsigned int bufferObject = -1;
	float* buffer = nullptr;
};
