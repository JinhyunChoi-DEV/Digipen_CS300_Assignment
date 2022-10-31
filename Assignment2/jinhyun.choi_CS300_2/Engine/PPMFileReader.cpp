#include "PPMFileReader.hpp"

void PPMFileReader::Read(std::string name, const char* fileName)
{
	std::string path = textureFilePath + std::string(fileName);
	PPM* ppm = new PPM();
	ppm->Read(name, path);

	ppmLists.insert(std::make_pair(name, ppm));
}
