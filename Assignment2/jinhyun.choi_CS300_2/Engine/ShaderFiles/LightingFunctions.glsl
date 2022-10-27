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

vec3 getAmbient(in float k, in vec3 intensity)
{
	return k*intensity;
}

vec3 getDiffuse(in float k, in vec3 intensity, in vec3 normalVector, in vec3 lightPos, in vec3 objectPos)
{
	vec3 normal = normalize(normalVector);
	vec3 light = normalize(lightPos - objectPos);
	float diff = max(dot(normal, light), 0.0);

	return k*diff*intensity;
}

vec3 getSpecular(in float k, in float exp, in vec3 intensity, in vec3 normalVector, in vec3 lightPos, in vec3 objectPos, in vec3 viewPos)
{
	vec3 normal = normalize(normalVector);
	vec3 light = normalize(lightPos - objectPos);
	vec3 view = normalize(viewPos - objectPos);
	vec3 reflection = 2.0 * dot(normal, light) * normal - light;
	float spec = pow(max(dot(reflection, view) , 0), exp);

	return k*intensity*spec;
}

vec3 getSpecularBlinn(in float k, in float exp, in vec3 intensity, in vec3 normalVector, in vec3 lightPos, in vec3 objectPos, in vec3 viewPos)
{
	vec3 normal = normalize(normalVector);
	vec3 light = normalize(lightPos - objectPos);
	vec3 view = normalize(viewPos - objectPos);
	vec3 half = normalize(light + view);
	float spec = pow(max(dot(normal, half), 0), exp);

	return k*intensity*spec;
}
