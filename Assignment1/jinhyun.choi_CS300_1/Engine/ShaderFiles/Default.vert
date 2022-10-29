#version 400 core

/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Default.vert
Purpose: The vertex shader to draw 3D model
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;

out vec3 diffusePos;
out vec3 outputNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	outputNormal = mat3(transpose(inverse(model))) * normal;
	diffusePos = vec3(model * vec4(position, 1.0));

	gl_Position = projection * view * model * vec3(position, 1.0);
}