#shader glsl

/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Texture.glsl
Purpose: The glsl file for texture function and data
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

uniform bool useTexture;
uniform uint mappingType;
uniform uint entityType;
uniform bool gpuMode;

uniform vec3 minVertex;
uniform	vec3 maxVertex;
uniform vec3 minVertexNormal;
uniform	vec3 maxVertexNormal;

uniform sampler2D ambientTexture;
uniform sampler2D specularTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D emissiveTexture;

uniform bool useAmbientTexture;
uniform bool useSpecularTexture;
uniform bool useDiffuseTexture;
uniform bool useEmissiveTexture;

uniform vec3 ambientColor;
uniform vec3 specularColor;
uniform vec3 diffuseColor;
uniform vec3 emissiveColor;

vec2 getTextureCoordinate(in vec3 vertexPos, in vec3 vertexNormal, in vec2 uvCoordinate)
{
	vec3 vertex = vec3(0.0);
	vec3 min = vec3(0.0);
	vec3 max = vec3(0.0);

	if(entityType == 0)
	{
		min = minVertex;
		max = maxVertex;
		vertex = vertexPos;
	}
	else
	{
		min = minVertexNormal;
		max = maxVertexNormal;
		vertex = normalize(vertexNormal);
	}

	vec2 uv = vec2(0.0f, 0.0f);

	if(!gpuMode)
		return uvCoordinate;

	if(mappingType == 0)
	{
		uv.x = (vertex.x-min.x)/(max.x-min.x);
		uv.y = (vertex.y-min.y)/(max.y-min.y);

		return uv;
	}
	else if(mappingType == 1)
	{
		float theta = degrees(atan(vertex.z, vertex.x)) + 180.0f;

		uv.x = theta/360.0f;
		uv.y = (vertex.y-min.y)/(max.y-min.y);

		return uv;
	}
	else if(mappingType == 2)
	{
		float x = vertex.x;
		float y = vertex.y;
		float z = vertex.z;
		float r = sqrt(x*x + y*y + z*z);
		float theta = degrees(atan(vertex.z, vertex.x)) + 180.0f;
		float phi = degrees(acos(y/r));

		uv.x = theta/360.f;
		uv.y = 1.0-(phi/180.0f);

		return uv;
	}
	else if(mappingType == 3)
	{
		vec3 absVector = abs(vertex);

		if(absVector.x >= absVector.y && absVector.x >= absVector.z)
		{
			if(vertex.x < 0.0f)
				uv.x = vertex.z/absVector.x;
			else
				uv.x = -vertex.z/absVector.x;

			uv.y = vertex.y/absVector.x;
		}
		else if(absVector.y >= absVector.x && absVector.y >= absVector.z)
		{
			if(vertex.y < 0.0f)
				uv.y = vertex.z/absVector.y;
			else
				uv.y = -vertex.z/absVector.y;

			uv.x = vertex.x/absVector.y;
		}
		else if(absVector.z >= absVector.x && absVector.z >= absVector.y)
		{
			if(vertex.z < 0.0f)
				uv.x = -vertex.x/absVector.z;
			else
				uv.x = vertex.x/absVector.z;

			uv.y = vertex.y/absVector.z;
		}

		return (uv+vec2(1))/2.0;
	}

	return vec2(0,0);
}