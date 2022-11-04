#shader vertex

/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: PhongLighting.vert
Purpose: The vertex shader to draw 3D model of PhongLighting
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 10/24/2022
End Header --------------------------------------------------------*/

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uvCoordinate;

out vec3 resultColor;

uniform vec3 objectColor;
uniform vec3 cameraPos;

void main()
{
	vec3 normalVector = mat3(transpose(inverse(transform.model))) * normal;
	vec3 objectPos = vec3(transform.model * vec4(position, 1.0));

	resultColor = vec3(uvCoordinate, 0.0f);
	//resultColor = calcuateLight(normalVector, objectPos, cameraPos, false) * objectColor;
	gl_Position = getPosition(position);
}