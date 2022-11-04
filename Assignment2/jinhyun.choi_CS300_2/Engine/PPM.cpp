#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

#include "PPM.hpp"

void PPM::Read(std::string path)
{
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
	auto size = width * height * 3;  // 3 -> r,g,b
	buffer = new float[size];

	if (mMagic != "P3")
		return;

	for(int i =0; i < size; ++i)
	{
		float value;
		file >> value;
		buffer[i] = value / (float)maxValue;
	}

	glGenTextures(1, &bufferObject);
	glBindTexture(GL_TEXTURE_2D, bufferObject);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, buffer);
	glGenerateMipmap(GL_TEXTURE_2D);
}
