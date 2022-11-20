/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: LightTypeHelper.hpp
Purpose: the helper function header for converting string data of LightType enum
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

#pragma once
#include <string>
#include <vector>
#include "Light.hpp"

inline std::string GetLightTypeString(LightType type) noexcept
{
	switch (type)
	{
	case LightType::Directional:
		return "Directional";

	case LightType::Point:
		return "Point";

	case LightType::Spotlight:
		return "Spotlight";

	default:
		return "";
	}
}

inline std::vector<std::string> GetLightTypesString() noexcept
{
	std::vector<std::string> result;
	auto count = static_cast<std::underlying_type<LightType>::type>(LightType::Count);
	for(int i = 0; i < count; ++i)
	{
		auto typeName = GetLightTypeString(static_cast<LightType>(i));
		if(typeName != "")
			result.push_back(typeName);
	}

	return result;
}

inline LightType GetTypeFromString(std::string typeName)
{
	if (typeName == "Directional")
		return LightType::Directional;

	if (typeName == "Point")
		return LightType::Point;

	if (typeName == "Spotlight")
		return LightType::Spotlight;
	
	return LightType::Count;
}