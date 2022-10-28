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

out vec3 resultColor;

uniform vec3 lightPosition;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 cameraPos;

void main()
{
	vec3 normalVector = mat3(transpose(inverse(transform.model))) * normal;
	vec3 objectPos = vec3(transform.model * vec4(position, 1.0));
	float ambientStrenght = 0.5;

	vec3 ambient = getAmbient(ambientStrenght, lightColor);
	vec3 diffuse = getDiffuse(1.0, lightColor, normalVector, lightPosition, objectPos);
	vec3 specular = getSpecular(1.0, 32.0, lightColor, normalVector, lightPosition, objectPos, cameraPos);

	vec3 result = (ambient + diffuse + specular) * objectColor;

	resultColor = result;
	gl_Position = getPosition(position);
}