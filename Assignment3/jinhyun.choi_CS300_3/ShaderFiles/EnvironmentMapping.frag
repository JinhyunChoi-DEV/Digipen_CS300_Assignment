#shader fragment

/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: EnvironmentMapping.frag
Purpose: the fragment shader for EnvironmentMapping
Language: GLSL
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/23/2022
End Header --------------------------------------------------------*/

uniform sampler2D topSide;
uniform sampler2D bottomSide;
uniform sampler2D frontSide;
uniform sampler2D backSide;
uniform sampler2D leftSide;
uniform sampler2D rightSide;

uniform vec3 viewPos;

in vec3 modelPos;
in vec3 vertexNormal;
in vec3 objectPos;

out vec4 outputColor;

vec3 getReflect();
vec3 getTexture(in vec3 vector);

void main()
{
	vec3 R = getReflect();
	vec3 color = getTexture(normalize(R));

	outputColor = vec4(color, 1);
}

vec3 getReflect()
{
	vec3 I = normalize(viewPos - objectPos);
	vec3 N = normalize(vertexNormal);

	return 2.0f * N * dot(N, I) - I;
}

vec3 getTexture(in vec3 vector)
{
	vec4 result;
	vec2 uv = vec2(0);
	vec3 absVector = abs(vector);

	if(absVector.x >= absVector.y && absVector.x >= absVector.z)
	{
		if(vector.x < 0)
		{
			uv.x = -vector.z/absVector.x;
			uv.y = vector.y/absVector.x;
			uv = (uv + vec2(1)) * 0.5f;

			result = texture(leftSide, uv);
		}
		else
		{
			uv.x = vector.z/absVector.x;
			uv.y = vector.y/absVector.x;
			uv = (uv + vec2(1)) * 0.5f;

			result = texture(rightSide, uv);
		}
	}
	else if(absVector.y >= absVector.x && absVector.y >= absVector.z)
	{
		if(vector.y < 0)
		{
			uv.x = vector.x / absVector.y;
			uv.y = -vector.z / absVector.y;
			uv = (uv + vec2(1)) * 0.5f;

			result = texture(bottomSide, uv);
		}
		else
		{
			uv.x = vector.x / absVector.y;
			uv.y = vector.z/absVector.y;
			uv = (uv + vec2(1)) * 0.5f;

			result = texture(topSide, uv);
		}
	}
	else if(absVector.z >= absVector.x && absVector.z >= absVector.y)
	{
		if(vector.z < 0)
		{
			uv.x = vector.x/absVector.z;
			uv.y = vector.y/absVector.z;
			uv = (uv + vec2(1)) * 0.5f;

			result = texture(frontSide, uv);
		}
		else
		{
			uv.x = -vector.x/absVector.z;
			uv.y = vector.y/absVector.z;
			uv = (uv + vec2(1)) * 0.5f;

			result = texture(backSide, uv);
		}
	}

	return result.rgb;
}
