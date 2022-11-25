#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Graphic.hpp"

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
	GLuint frameBuffer = 0;
	GLuint depthRenderBuffer = 0;
	std::vector<GLuint> renderedTexture;
	std::vector<GLenum> drawBuffers;
};
