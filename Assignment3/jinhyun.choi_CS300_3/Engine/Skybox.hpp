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
	SkyBox(glm::vec2 windowSize);

	void CreateRenderToTexture();
	void Draw();

private:
	void CreateImage(int index);
	void CreateFrameBuffer();

	GLuint frameBuffer = 0;
	GLuint renderedTexture = 0;
	GLuint depthRenderBuffer = 0;
	int const cubeMapCount = 6;
	std::string meshName = "cube2.obj";
	Mesh* mesh;
	Object* obj;

	int width = -1;
	int height = -1;
	std::vector<std::string> cubeSideName = { "top", "bottom", "right", "left", "back", "front" };
	std::unordered_map<std::string, Image> skyBoxTexture;
	std::vector<GLenum> drawBuffers;
};
