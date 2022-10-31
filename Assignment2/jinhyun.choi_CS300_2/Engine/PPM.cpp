#include <fstream>
#include <sstream>
#include <iostream>

#include "PPM.hpp"

void PPM::Read(std::string name, std::string path)
{
	this->name = name;
	this->path = path;

	std::ifstream file(path, std::ifstream::binary);

	if (!file.is_open())
	{
		std::cout << "PPM File Load Error" << std::endl;
		return;
	}

	file >> mMagic;
	file.seekg(1, file.cur);
	char c;
	file.get(c);
	if(c == '#')
	{
		while(c != '\n')
		{
			file.get(c);
		}
	}
	else
	{
		file.seekg(-1, file.cur);
	}

	file >> width >> height >> maxValue;
	auto size = width * height * 3;
	buffer = new float[size]; // 3 -> r,g,b

	if (mMagic != "P3")
		return;

	for(int i =0; i < size; ++i)
	{
		float value;
		file >> value;
		buffer[i] = value / (float)maxValue;
	}
}
