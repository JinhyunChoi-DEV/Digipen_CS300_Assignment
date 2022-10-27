#version 400 core

/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Default.frag
Purpose: The fragment shader to draw 3D model
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

in vec3 diffusePos;
in vec3 outputNormal;

out vec4 outputColor;

uniform vec3 lightPosition;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{

	float ambientStrenght = 0.1;
	vec3 ambient = ambientStrenght * lightColor;

	vec3 normalizedNormal = normalize(outputNormal);
	vec3 lightDirection = normalize(lightPosition - diffusePos);
	float diff = max(dot(normalizedNormal, lightDirection), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient  + diffuse) * objectColor;
	outputColor = vec4(result, 1.0);
}