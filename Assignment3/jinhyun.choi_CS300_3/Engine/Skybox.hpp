#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class Image;
class Mesh;
class Object;

class SkyBox
{
public:
	SkyBox();

	void Draw();
private:
	void CreateImage(int index);

	int const cubeMapCount = 6;
	std::string meshName = "cube2.obj";
	Mesh* mesh;
	Object* obj;

	std::vector<std::string> cubeSideName = { "top", "bottom", "right", "left", "back", "front" };
	std::unordered_map<std::string, Image> skyBoxTexture;

	// FrameBuffer
	unsigned frameBuffer = 0;
	unsigned depthRenderBuffer = 0;
	std::vector<unsigned> renderedTexture;
	std::vector<unsigned> drawBuffers;
};
