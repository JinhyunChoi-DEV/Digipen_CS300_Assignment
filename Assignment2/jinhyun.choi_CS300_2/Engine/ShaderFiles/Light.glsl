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
    uint type;			// 0, Directional=0, Point=1, Spotlight=2

	vec3 position;		// 16
	vec3 direction;		// 32
	vec3 ambient;		// 48
	vec3 diffuse;		// 64

	vec3 specular;		// 80
	float cutOff;		// 92

	float outerCutOff;	// 96
	float constant;		// 100
	float linear;		// 104
	float quadratic;	// 108

	/* 
	Total = type(16) + position(16) + 
	direction(16) + ambient(16) + diffuse(16) +
	specular(12) + cutoff(4) + 
	outerCutOff(4) + constant(4) + linear(4) + quadratic(4) = 112
	*/
} light;

vec3 getAmbient(in float k)
{
	return light.ambient * k;
}

vec3 getDiffuse(in float k, in vec3 normalVector, in vec3 objectPos)
{
	vec3 normal = normalize(normalVector);

	vec3 lightDir;
	if (light.type == 0)
	{
		lightDir = normalize(-light.direction);
	}
	else
	{
		lightDir = normalize(light.position - objectPos);
	}

	float diff = max(dot(normal, lightDir), 0.0);

	return light.diffuse * k * diff;
}

vec3 getSpecular(in float k, in float exp, in vec3 normalVector, in vec3 objectPos, in vec3 viewPos)
{
	vec3 normal = normalize(normalVector);
	vec3 lightDir;
	if (light.type == 0)
	{
		lightDir = normalize(-light.direction);
	}
	else
	{
		lightDir = normalize(light.position - objectPos);
	}
	vec3 view = normalize(viewPos - objectPos);
	vec3 reflection = 2.0 * dot(normal, lightDir) * normal - lightDir;
	float spec = pow(max(dot(reflection, view) , 0), exp);

	return light.specular * k * spec;
}

vec3 getSpecularBlinn(in float k, in float exp, in vec3 normalVector, in vec3 objectPos, in vec3 viewPos)
{
	vec3 normal = normalize(normalVector);
	vec3 lightDir;
	if (light.type == 0)
	{
		lightDir = normalize(-light.direction);
	}
	else
	{
		lightDir = normalize(light.position - objectPos);
	}
	vec3 view = normalize(viewPos - objectPos);
	vec3 halfVec = normalize(lightDir + view);
	float spec = pow(max(dot(normal, halfVec), 0), exp);

	return light.specular * k * spec;
}

vec3 getColorResult(in vec3 ambient, in vec3 diffuse, in vec3 specular, in vec3 objectPos, in vec3 viewPos)
{
	if (light.type == 0)
	{
		return ambient + diffuse + specular;
	}
	else if (light.type == 1)
	{
		float distance = length(light.position - objectPos);
		float att = 1.0/(light.constant + light.linear * distance + light.quadratic * (distance*distance));
		return (ambient + diffuse + specular) * att ;
	}
	else if (light.type == 2)
	{
		vec3 lightDir = normalize(light.position - objectPos);
		vec3 viewDir = normalize(viewPos - objectPos);
		float angle = dot(lightDir, viewDir);
		float innerAngle = cos(radians(light.cutOff));
		float outerAngle = cos(radians(light.outerCutOff));
		float spot = (outerAngle - angle)/(outerAngle - innerAngle);
		float distance = length(light.position - objectPos);
		float att = 1.0/(light.constant + light.linear * distance + light.quadratic * (distance*distance));

		//return att * (ambient + diffuse + specular);
		return att * ambient + att * spot * (diffuse + specular);
	}

	return vec3(0.0, 0.0, 0.0);
}