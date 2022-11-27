/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Skybox.hpp
Purpose: the header file for Skybox data and function
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/27/2022
End Header --------------------------------------------------------*/

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
