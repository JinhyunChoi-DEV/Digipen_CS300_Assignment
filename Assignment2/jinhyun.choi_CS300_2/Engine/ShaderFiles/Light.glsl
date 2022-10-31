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

#define LIGHT_MAX 16

struct Light
{
	uint type;			// 0, Directional=0, Point=1, Spotlight=2
	vec3 position;		// 16
	vec3 direction;		// 32
	vec3 ambient;		// 48
	vec3 diffuse;		// 64

	vec3 specular;		// 80
	float innerAngle;	// 92

	float outerAngle;	// 96
	float constant;		// 100
	float linear;		// 104
	float quadratic;	// 108

	float fallOut;		// 112
	/* 
	Total = type(16) + position(16) + 
	direction(16) + ambient(16) + diffuse(16) +
	specular(12) + innerAngle(4) + 
	outerAngle(4) + constant(4) + linear(4) + quadratic(4) +
	fallOut(16) = 128
	*/
};

layout(std140, binding = 1) uniform Lights
{
	uint lightCount;			// 16
	Light light[LIGHT_MAX];		// 128 each one
} lights;

vec3 calcuateLight(in vec3 normalVector, in vec3 objectPos, in vec3 viewPos, in bool isBlinn);
vec3 getAmbient(in float k, Light light);
vec3 getDiffuse(in float k, in vec3 normalVector, in vec3 objectPos, Light light);
vec3 getSpecular(in float k, in float exp, in vec3 normalVector, in vec3 objectPos, in vec3 viewPos,  Light light);
vec3 getSpecularBlinn(in float k, in float exp, in vec3 normalVector, in vec3 objectPos, in vec3 viewPos, Light light);
vec3 getColorResult(in vec3 ambient, in vec3 diffuse, in vec3 specular, in vec3 objectPos, in vec3 viewPos, Light light);

vec3 calcuateLight(in vec3 normalVector, in vec3 objectPos, in vec3 viewPos, in bool isBlinn)
{
	float ambientStrenght = 0.1;
	float exp = 32.0;

	vec3 result = vec3(0);
	for(int i=0; i<lights.lightCount; ++i)
	{
		vec3 ambient = getAmbient(ambientStrenght, lights.light[i]);
		vec3 diffuse = getDiffuse(1.0, normalVector, objectPos, lights.light[i]);
		vec3 specular;
		if(isBlinn)
			specular = getSpecularBlinn(1.0, exp, normalVector, objectPos, viewPos, lights.light[i]);
		else
			specular = getSpecular(1.0, exp, normalVector, objectPos, viewPos, lights.light[i]);

		result += getColorResult(ambient, diffuse, specular, objectPos, viewPos, lights.light[i]);
	}

	return result;
}

vec3 getAmbient(in float k, Light light)
{
	return light.ambient * k;
}

vec3 getDiffuse(in float k, in vec3 normalVector, in vec3 objectPos, Light light)
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

vec3 getSpecular(in float k, in float exp, in vec3 normalVector, in vec3 objectPos, in vec3 viewPos,  Light light)
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

vec3 getSpecularBlinn(in float k, in float exp, in vec3 normalVector, in vec3 objectPos, in vec3 viewPos, Light light)
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

vec3 getColorResult(in vec3 ambient, in vec3 diffuse, in vec3 specular, in vec3 objectPos, in vec3 viewPos, Light light)
{
	if (light.type == 0) // Directional
	{
		return ambient + diffuse + specular;
	}
	else if (light.type == 1) // Point
	{
		float distance = length(light.position - objectPos);
		float att = min(1.0/(light.constant + light.linear * distance + light.quadratic * (distance*distance)), 1.0);
		return (ambient + diffuse + specular) * att ;
	}
	else if (light.type == 2) //Spot
	{
		float spotEffect = 0;
		vec3 lightDir= normalize(light.direction);
		vec3 vertextoLightDir = normalize(objectPos - light.position);
		float angle = dot(lightDir, vertextoLightDir);
		float phi = cos(radians(light.outerAngle));
		float theta = cos(radians(light.innerAngle));
		float distance = length(light.position - objectPos);
		float att = min(1.0/(light.constant + light.linear * distance + light.quadratic * (distance*distance)), 1.0);

		if(angle < phi)
		{
			spotEffect = 0;
		}
		else if(angle > theta)
		{
			spotEffect = 1;
		}
		else
		{
			spotEffect = pow(clamp((angle-phi)/(theta-phi), 0.0, 1.0), light.fallOut);
		}
		
		return att * spotEffect * (ambient+diffuse + specular);
	}
	return vec3(0.0, 0.0, 0.0);
}