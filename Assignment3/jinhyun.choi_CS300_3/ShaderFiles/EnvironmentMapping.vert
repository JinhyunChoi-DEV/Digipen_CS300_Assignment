#shader vertex

/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: EnvironmentMapping.vert
Purpose: the vertex shader for EnvironmentMapping
Language: GLSL
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/23/2022
End Header --------------------------------------------------------*/

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uvCoordinate;

out vec3 modelPos;
out vec3 objectPos;
out vec3 vertexNormal;
out vec2 cpuUV;

void main()
{
	modelPos = position;
	vertexNormal = mat3(transpose(inverse(transform.model))) * normal;
	objectPos = vec3(transform.model * vec4(position, 1.0));
	cpuUV = uvCoordinate;

	gl_Position = getPosition(position);
}