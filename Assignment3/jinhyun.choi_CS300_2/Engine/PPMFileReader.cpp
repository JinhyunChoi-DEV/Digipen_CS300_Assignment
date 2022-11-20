/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: PPMFileReader.cpp
Purpose: the cpp file for reading PPMFiles
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

#include "PPMFileReader.hpp"

PPMFileReader* PPMREADER = nullptr;

PPMFileReader::PPMFileReader()
{
	if(PPMREADER ==	nullptr)
		PPMREADER = this;
}

bool PPMFileReader::IsExist(std::string file)
{
	return ppmLists.find(file) != ppmLists.end();
}

void PPMFileReader::Read(std::string fileName)
{
	if (IsExist(fileName))
		return;

	std::string path = textureFilePath + fileName;
	PPM ppm;
	ppm.Read(path);

	ppmLists.insert(std::make_pair(fileName, ppm));
}

PPM PPMFileReader::Get(std::string file)
{
	return ppmLists[file];
}
