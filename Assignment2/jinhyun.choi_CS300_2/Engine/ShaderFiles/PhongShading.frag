#shader fragment

/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: PhongShading.frag
Purpose: The fragment shader to draw 3D model
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 10/24/2022
End Header --------------------------------------------------------*/

in vec3 objectPos;
in vec3 outputNormal;
out vec4 outputColor;

uniform vec3 lightPosition;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 cameraPos;

void main()
{
	float ambientStrenght = 0.5;

	vec3 amient = getAmbient(ambientStrenght, lightColor);
	vec3 diffuse = getDiffuse(1.0, lightColor, outputNormal, lightPosition, objectPos);
	vec3 specular = getSpecular(1.0, 32.0, lightColor, outputNormal, lightPosition, objectPos, cameraPos);
	vec3 resultColor = (amient + diffuse + specular) * objectColor;

	outputColor = vec4(resultColor, 1.0);
}