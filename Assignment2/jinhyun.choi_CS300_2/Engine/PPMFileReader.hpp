#pragma once
#include <string>
#include <unordered_map>

#include "PPM.hpp"

class PPMFileReader
{
public:
	PPMFileReader();

	bool IsExist(std::string file);
	void Read(std::string fileName);
	PPM Get(std::string file);

private:
	std::unordered_map<std::string, PPM> ppmLists;
	std::string textureFilePath = "../Textures/";
};

extern PPMFileReader* PPMREADER;