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
