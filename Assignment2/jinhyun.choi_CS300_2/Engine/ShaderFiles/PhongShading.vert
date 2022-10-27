#shader vertex

/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: PhongShading.vert
Purpose: The vertex shader to draw 3D model
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 10/24/2022
End Header --------------------------------------------------------*/

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;

out vec3 objectPos;
out vec3 outputNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	outputNormal = mat3(transpose(inverse(model))) * normal;
	objectPos = vec3(model * vec4(position, 1.0));

	gl_Position = projection * view * model * vec4(position, 1.0);
}