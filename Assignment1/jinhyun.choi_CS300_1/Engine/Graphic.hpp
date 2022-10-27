/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Graphic.hpp
Purpose: Header of graphic class
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once
#include "Camera.hpp"
#include "EngineInterface.hpp"
#include "ShaderManager.hpp"
#include "VertexObjectManager.hpp"

class Mesh;
class Transform;

class Graphic : public EngineInterface
{
public:
	Graphic();

	void Initialize() override;
	void Update() override;
	void Terminate() override;

	void SetViewSize(glm::vec2 windowSize);
	void SetCamera(Camera* camera);
	void CompileShader(std::string vertexFile, std::string fragFile, std::string name);

	Shader* GetShader(std::string name) const;
	Camera* GetCamera() const { return camera; }

private:
	void Draw();
	void DrawObject(const Transform* transform, const Mesh* mesh);
	void DrawLine(const Transform* transform, const Mesh* mesh);
	void DrawVertexNormal(Transform* transform, const Mesh* mesh);
	void DrawFaceNormal(Transform* transform, const Mesh* mesh);
	void DrawNormalGUI();

	Camera* camera;
	ShaderManager* shaderManager;
	VertexObjectManager* vertexObjectManager;
	bool drawVertexNormal, drawFaceNormal;
};

extern Graphic* GRAPHIC;