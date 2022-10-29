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

layout(std140, binding = 1) uniform Light
{
	vec3 position;
	vec3 direction;

	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
} light;

vec3 getAmbient(in float k)
{
	return light.ambient * k;
}

vec3 getDiffuse(in float k, in vec3 normalVector)
{
	vec3 normal = normalize(normalVector);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);

	return light.diffuse * k * diff;
}

vec3 getSpecular(in float k, in float exp, in vec3 normalVector, in vec3 objectPos, in vec3 viewPos)
{
	vec3 normal = normalize(normalVector);
	vec3 lightDir = normalize(-light.direction);
	vec3 view = normalize(viewPos - objectPos);
	vec3 reflection = 2.0 * dot(normal, lightDir) * normal - lightDir;
	float spec = pow(max(dot(reflection, view) , 0), exp);

	return light.specular * k * spec;
}

vec3 getSpecularBlinn(in float k, in float exp, in vec3 normalVector, in vec3 objectPos, in vec3 viewPos)
{
	vec3 normal = normalize(normalVector);
	vec3 lightDir = normalize(-light.direction);
	vec3 view = normalize(viewPos - objectPos);
	vec3 halfVec = normalize(lightDir + view);
	float spec = pow(max(dot(normal, halfVec), 0), exp);

	return light.specular * k * spec;
}