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
	int width, height, maxValue = -1;
	unsigned int bufferObject = -1;
	float* buffer = nullptr;
};
