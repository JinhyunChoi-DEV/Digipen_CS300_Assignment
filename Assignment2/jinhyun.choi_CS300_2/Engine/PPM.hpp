#pragma once
#include <string>

class PPM
{
public:
	void Read(std::string name, std::string path);

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	int GetMax() const { return maxValue; }
	float* GetBuffer() const { return buffer; }

private:
	std::string mMagic, path, name;
	int width, height, maxValue;
	float* buffer = nullptr;
};
