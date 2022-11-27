/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: TextureTypeHelper.hpp
Purpose: the helper function header for converting string data of TextureMappingType and TextureEntityType enums
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

#pragma once
#include <string>
#include <vector>

#include "Texture.hpp"

inline std::string GetTextureMappingTypeString(TextureMappingType type) noexcept
{
	switch (type)
	{
	case TextureMappingType::Planar:
		return "Planar";

	case TextureMappingType::Cylindrical:
		return "Cylindrical";

	case TextureMappingType::Spherical:
		return "Spherical";

	case TextureMappingType::Cube:
		return "Cube";

	default:
		return "";
	}
}

inline std::vector<std::string> GetTextureMappingTypesString() noexcept
{
	std::vector<std::string> result;
	auto count = static_cast<std::underlying_type<TextureMappingType>::type>(TextureMappingType::Count);
	for (int i = 0; i < count; ++i)
	{
		auto typeName = GetTextureMappingTypeString(static_cast<TextureMappingType>(i));
		if (typeName != "")
			result.push_back(typeName);
	}

	return result;
}

inline TextureMappingType GetTextureMappingFromString(std::string typeName)
{
	if (typeName == "Planar")
		return TextureMappingType::Planar;

	if (typeName == "Cylindrical")
		return TextureMappingType::Cylindrical;

	if (typeName == "Spherical")
		return TextureMappingType::Spherical;

	if (typeName == "Cube")
		return TextureMappingType::Cube;

	return TextureMappingType::Count;
}

inline std::string GetTextureEntityTypeString(TextureEntityType type) noexcept
{
	switch (type)
	{
	case TextureEntityType::VertexPosition:
		return "VertexPosition";

	case TextureEntityType::VertexNormal:
		return "VertexNormal";

	default:
		return "";
	}
}

inline std::vector<std::string> GetTextureEntityTypesString() noexcept
{
	std::vector<std::string> result;
	auto count = static_cast<std::underlying_type<TextureEntityType>::type>(TextureEntityType::Count);
	for (int i = 0; i < count; ++i)
	{
		auto typeName = GetTextureEntityTypeString(static_cast<TextureEntityType>(i));
		if (typeName != "")
			result.push_back(typeName);
	}

	return result;
}

inline TextureEntityType GetTextureEntityTypeFromString(std::string typeName)
{
	if (typeName == "VertexPosition")
		return TextureEntityType::VertexPosition;

	if (typeName == "VertexNormal")
		return TextureEntityType::VertexNormal;

	return TextureEntityType::Count;
}