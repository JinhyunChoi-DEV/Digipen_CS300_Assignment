#pragma once
#include <string>
#include <unordered_map>

#include "PPM.hpp"

class PPMFileReader
{
public:
	void Read(std::string name, const char* fileName);

private:
	std::unordered_map<std::string, PPM*> ppmLists;
	std::string textureFilePath = "../Textures/";
};
