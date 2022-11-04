#shader glsl

/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: LightingFunctions.glsl
Purpose: The vertex shader to draw line
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

layout(std140, binding = 0) uniform Transform
{
	mat4 model;
	mat4 view;
	mat4 projection;
} transform;

vec4 getPosition(vec3 vertex)
{
	return transform.projection * transform.view * transform.model * vec4(vertex, 1.0);
}