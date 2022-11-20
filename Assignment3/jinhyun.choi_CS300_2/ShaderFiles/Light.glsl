#shader glsl

/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Light.glsl
Purpose: the glsl code for lighting data and functions
Language: GLSL
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
	float fallOut;		// 104
	/* 
	Total = type(16) + position(16) + 
	direction(16) + ambient(16) + diffuse(16) +
	specular(12) + innerAngle(4) + 
	outerAngle(8) + fallOut(8) = 112
	*/
};

layout(std140, binding = 1) uniform Lights
{
	uint lightCount;			// 4
	float near;					// 4 -> 8
	float far;					// 8 -> 16

	vec3 attenuationConstants;	// 16 -> 32
	vec3 globalAmbientColor;	// 16 -> 48
	vec3 fogColor;				// 16 -> 64
	Light light[LIGHT_MAX];		// 108 each one
} lights;

float constStrenght = 0.5;
vec3 calcuateLight(in vec3 normalVector, in vec3 objectPos, in vec3 viewPos, in vec2 uv, in bool isBlinn);
vec3 getAmbient(in vec2 uv, in Light light);
vec3 getDiffuse(in vec2 uv, in vec3 normalVector, in vec3 objectPos, Light light);
vec3 getSpecular(in vec2 uv, in vec3 normalVector, in vec3 objectPos, in vec3 viewPos,  Light light);
vec3 getSpecularBlinn(in vec2 uv, in vec3 normalVector, in vec3 objectPos, in vec3 viewPos, Light light);
vec3 getLocalLight(in vec3 ambient, in vec3 diffuse, in vec3 specular, in vec3 objectPos, in vec3 viewPos, Light light);

vec3 calcuateLight(in vec3 normalVector, in vec3 objectPos, in vec3 viewPos, in vec2 uv, in bool isBlinn)
{
	float S = (lights.far - length(viewPos - objectPos))/(lights.far - lights.near);

	vec3 emissive = vec3(0);
	if(useTexture)
	{
		if(useEmissiveTexture)
			emissive = texture(emissiveTexture, uv).rgb;
		else
			emissive = emissiveColor;
	}

	vec3 lightsResult = vec3(0);

	for(int i=0; i<lights.lightCount; ++i)
	{
		vec3 ambient = getAmbient(uv, lights.light[i]);
		vec3 diffuse = getDiffuse(uv, normalVector, objectPos, lights.light[i]);
		vec3 specular;
		if(isBlinn)
			specular = getSpecularBlinn(uv, normalVector, objectPos, viewPos, lights.light[i]);
		else
			specular = getSpecular(uv, normalVector, objectPos, viewPos, lights.light[i]);

		vec3 localLight = getLocalLight(ambient, diffuse, specular, objectPos, viewPos, lights.light[i]);
		lightsResult += max(localLight, 0);
	}

	vec3 I_local = vec3(0.0f);
	if(useTexture && useAmbientTexture)
		I_local = emissive + lights.globalAmbientColor * ambientColor + lightsResult;
	else
		I_local = emissive + lights.globalAmbientColor * constStrenght + lightsResult;

	return S*I_local + (1.0-S)*lights.fogColor;
}

vec3 getAmbient(in vec2 uv, in Light light)
{
	if(useTexture)
	{
		if(useAmbientTexture)
			return light.ambient * ambientColor * texture(ambientTexture, uv).rgb;
		else
			return light.ambient * ambientColor;
	}
	else
	{
		return light.ambient * constStrenght;
	}
}

vec3 getDiffuse(in vec2 uv, in vec3 normalVector, in vec3 objectPos, Light light)
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

	if(useTexture)
	{
		if(useDiffuseTexture)
			return light.diffuse * diff * diffuseColor * texture(diffuseTexture, uv).rgb;
		else
			return light.diffuse * diff * diffuseColor;		
	}
	else
	{
		return light.diffuse * diff;
	}
	
}

vec3 getSpecular(in vec2 uv, in vec3 normalVector, in vec3 objectPos, in vec3 viewPos,  Light light)
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

	if(useTexture)
	{	
		if (useSpecularTexture)
		{
			float spec = pow(max(dot(reflection, view) , 0), specularColor.r + specularColor.r);
			return light.specular * spec * specularColor * texture(specularTexture, uv).rgb;
		}
		else
		{
			float spec = pow(max(dot(reflection, view) , 0), specularColor.r*specularColor.r);
			return light.specular * spec * specularColor;
		}
	}
	else
	{
		float spec = pow(max(dot(reflection, view) , 0), 32.0);
		return light.specular * spec;
	}
	
}

vec3 getSpecularBlinn(in vec2 uv, in vec3 normalVector, in vec3 objectPos, in vec3 viewPos, Light light)
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


	if(useTexture)
	{
		if (useSpecularTexture)
		{
			float spec = pow(max(dot(normal, halfVec) , 0), specularColor.r * specularColor.r);
			return light.specular * spec * specularColor * texture(specularTexture, uv).rgb;
		}
		else
		{
			float spec = pow(max(dot(normal, halfVec) , 0), specularColor.r * specularColor.r);
			return light.specular * spec * specularColor;
		}
	}
	else
	{
		float spec = pow(max(dot(normal, halfVec) , 0), 32.0);
		return light.specular * spec;
	}
}

vec3 getLocalLight(in vec3 ambient, in vec3 diffuse, in vec3 specular, in vec3 objectPos, in vec3 viewPos, Light light)
{
	if (light.type == 0) // Directional
	{
		return ambient + diffuse + specular;
	}
	else if (light.type == 1) // Point
	{
		float distance = length(light.position - objectPos);
		float att = min(1.0 / (lights.attenuationConstants.x + (lights.attenuationConstants.y * distance) + (lights.attenuationConstants.z * (distance*distance))), 1.0);
		return (ambient+diffuse+specular)*att;
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
		float att = min(1.0 / (lights.attenuationConstants.x + (lights.attenuationConstants.y * distance) + (lights.attenuationConstants.z * (distance*distance))), 1.0);

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
			spotEffect = pow((angle-phi)/(theta-phi), light.fallOut);
		}
		
		return att * spotEffect * (ambient + diffuse + specular);
	}
	return vec3(0.0, 0.0, 0.0);
}