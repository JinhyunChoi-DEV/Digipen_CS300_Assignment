#pragma once

#include <vector>
#include <glm/glm.hpp>

class Shader;
class Camera;
class Transform;
class Mesh;

enum class VisualizeType
{
	OnlyReflect = 0, OnlyRefract, BothEnvironment, EnvironmentWithPhong, OnlyPhong, Count
};

class EnvironmentMapping
{
public:
	EnvironmentMapping(glm::vec2 size);

	void CheckRenterToTexture();
	void Bind(int index);
	void UnBind();
	void Update(Shader* shader);

	int width, height;
	VisualizeType visualizeType;
	float refractiveIndex, fresnelPower, colorMixRatio;

private:
	void CreateFrameBuffer();

	Camera* cam;
	Mesh* mesh;
	Transform* baseTransform;

	bool initialize = false;
	int textureWidth, textureHeight;

	unsigned frameBufferObject;
	unsigned depthRenderBuffer;
	std::vector<unsigned> renderToTexture;

	std::vector<std::string> samplerName = {
		"topSide", "bottomSide",
		"frontSide", "backSide",
		"leftSide", "rightSide"
	};

	std::vector<unsigned> drawBuffers;
};

inline std::string GetVisualizeTypeString(VisualizeType type) noexcept
{
	switch (type)
	{
	case VisualizeType::OnlyReflect:
		return "Only Reflect";

	case VisualizeType::OnlyRefract:
		return "Only Refract";

	case VisualizeType::BothEnvironment:
		return "Both Environment Mapping";

	case VisualizeType::EnvironmentWithPhong:
		return "Environment Mapping With Phong";

	case VisualizeType::OnlyPhong:
		return "Only Phong";

	default:
		return "";
	}
}

inline std::vector<std::string> GetVisualizeTypeString() noexcept
{
	std::vector<std::string> result;
	auto count = static_cast<std::underlying_type<VisualizeType>::type>(VisualizeType::Count);
	for (int i = 0; i < count; ++i)
	{
		auto typeName = GetVisualizeTypeString(static_cast<VisualizeType>(i));
		if (typeName != "")
			result.push_back(typeName);
	}

	return result;
}

inline VisualizeType GetVisualizeTypeFromString(std::string typeName)
{
	if (typeName == "Only Reflect")
		return VisualizeType::OnlyReflect;

	if (typeName == "Only Refract")
		return VisualizeType::OnlyRefract;

	if (typeName == "Both Environment Mapping")
		return VisualizeType::BothEnvironment;

	if (typeName == "Environment Mapping With Phong")
		return VisualizeType::EnvironmentWithPhong;

	if (typeName == "Only Phong")
		return VisualizeType::OnlyPhong;

	return VisualizeType::Count;
}
