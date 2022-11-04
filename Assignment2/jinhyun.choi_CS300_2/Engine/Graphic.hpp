/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Graphic.hpp
Purpose: Header of graphic class
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#pragma once
#include "Camera.hpp"
#include "EngineInterface.hpp"
#include "ShaderManager.hpp"
#include "UniformBlockObjectManager.hpp"
#include "VertexObjectManager.hpp"
#include "Texture.hpp"

class Mesh;
class Transform;
class Object;

class Graphic : public EngineInterface
{
public:
	Graphic();

	void Initialize() override;
	void Update() override;
	void Terminate() override;

	void SetViewSize(glm::vec2 windowSize);
	void SetCamera(Camera* camera);
	void CompileShader(std::string name, const char* path, ...);
	void SetDrawNormal(bool drawVertex, bool drawFace);
	void RebindTexture(Object* object);
	void SetGlobalLightInfo(glm::vec3 attenuation, glm::vec3 globalAmbient, glm::vec3 fog, float near, float far);

	Shader* GetShader(std::string name) const;
	Camera* GetCamera() const { return camera; }
	glm::vec3 GetFogColor() const { return fogColor; }
	glm::vec3 GetGlobalAmbient() const { return globalAmbientColor; }
	glm::vec3 GetAttenuation() const { return attenuationConstants; }
	float GetFogMin() const { return fogMin; }
	float GetFogMax() const { return fogMax; }

private:
	void UpdateLight();
	void Draw();
	void DrawModel(Object* object);
	void DrawObject(Object* object);
	void DrawSolid(Object* object);
	void DrawLine(Object* object);
	void DrawLight(Object* object);
	void DrawVertexNormal(Object* object);
	void DrawFaceNormal(Object* object);
	void TextureBind(Mesh* mesh, Texture* texture);

	Camera* camera;
	ShaderManager* shaderManager;
	VertexObjectManager* vertexObjectManager;
	UniformBlockObjectManager* uboManager;
	glm::vec3 fogColor = { 0.0f, 0.0f, 0.0f};
	glm::vec3 globalAmbientColor = {0, 0, 0.0f};
	glm::vec3 attenuationConstants = {1.0f, 0.22f, 0.2f};
	bool drawVertexNormal, drawFaceNormal;
	float fogMin = 0.1f;
	float fogMax = 25.0f;
};

extern Graphic* GRAPHIC;