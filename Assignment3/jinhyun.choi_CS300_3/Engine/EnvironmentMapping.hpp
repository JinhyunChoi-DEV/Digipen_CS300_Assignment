#pragma once
#include <vector>
#include <glad/glad.h>
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"

class EnvironmentMapping
{
public:
	EnvironmentMapping(glm::vec2 size);

	void Bind(int index);
	void UnBind();
	void UpdateMappingTexture(Shader* shader);

private:
	void CreateFrameBuffer();

	Camera* cam;
	Mesh* mesh;
	Transform* baseTransform;

	int width, height;

	GLuint frameBufferObject;
	GLuint depthRenderBuffer;
	std::vector<GLuint> renderToTexture;

	std::vector<std::string> samplerName = {
		"topSide", "bottomSide",
		"frontSide", "backSide",
		"leftSide", "rightSide"
	};

	std::vector<GLenum> drawBuffers = {
		GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT
	};
};
