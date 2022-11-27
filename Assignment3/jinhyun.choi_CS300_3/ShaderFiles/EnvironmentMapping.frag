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

// new Frame Buffer Texture
uniform sampler2D topSide;
uniform sampler2D bottomSide;
uniform sampler2D frontSide;
uniform sampler2D backSide;
uniform sampler2D leftSide;
uniform sampler2D rightSide;

// phong shading field
uniform float colorMixRatio;

// position
uniform vec3 viewPos;

// flag
uniform int type; //0:OnlyReflect, 1:OnlyRefract, 2:BothEnvironment, 3:EnvironmentWithPhong, 4:OnlyPhong

// refract component
uniform float refractiveIndex;
uniform float fresnelPower;

in vec3 modelPos;
in vec3 vertexNormal;
in vec3 objectPos;
in vec2 cpuUV;
out vec4 outputColor;

vec3 getReflectColor();
vec3 getRefractColor();
vec3 getMixColor();
vec3 getRefractVector(in float eta);
vec3 getTexture(in vec3 vector);
vec3 getPhongShadingColor();

void main()
{
	vec3 color = vec3(0);

	if(type == 0)
	{
		color = mix(emissiveColor, getReflectColor(), colorMixRatio);
	}
	else if(type == 1)
	{
		color = mix(emissiveColor, getRefractColor(), colorMixRatio);
	}
	else if(type == 2)
	{
		color = mix(emissiveColor, getMixColor(), colorMixRatio);
	}
	else if(type == 3)
	{
		color = mix(getPhongShadingColor(), getMixColor(), colorMixRatio);
	}
	else
	{
		color = getPhongShadingColor();
	}

	outputColor = vec4(color, 1);
}

vec3 getReflectColor()
{
	vec3 I = normalize(viewPos - objectPos);
	vec3 N = normalize(vertexNormal);
	vec3 Reflect = 2.0f * N * dot(N, I) - I;

	return getTexture(Reflect);
}

vec3 getRefractColor()
{
	float eta = 1.0f / refractiveIndex;
	return getTexture(getRefractVector(eta));
}

vec3 getMixColor()
{
	vec3 reflectColor = getReflectColor();

	vec3 I = normalize(viewPos - objectPos);
	vec3 N = normalize(vertexNormal);
	float d = dot(N,I);

	float K = 1.0f / refractiveIndex;
	float F = ((1.0-K)*(1.0-K)) / ((1.0+K)*(1.0+K));
    float Ratio = F + (1.0 - F) * pow(1.0 - dot(N, I), fresnelPower);

	vec3 refractVec[3];
	refractVec[0] = getRefractVector(K*0.99f);
	refractVec[1] = getRefractVector(K);
	refractVec[2] = getRefractVector(K*1.01f);

	vec3 refractColor[3];
	refractColor[0] = getTexture(refractVec[0]);
	refractColor[1] = getTexture(refractVec[1]);
	refractColor[2] = getTexture(refractVec[2]);

	vec3 refractionColor = vec3(0);
	refractionColor.r = refractColor[0].r;
	refractionColor.g = refractColor[1].g;
	refractionColor.b = refractColor[2].b;

	return mix(refractionColor, refractColor[1], Ratio);
}

vec3 getRefractVector(in float eta)
{
	vec3 I = normalize(viewPos - objectPos);
	vec3 N = normalize(vertexNormal);
	float d = dot(N,I);

	return (eta*d - sqrt(1-pow(eta,2)*(1-pow(d,2))))*N - eta*I;
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

vec3 getPhongShadingColor()
{
	vec2 uv = getTextureCoordinate(modelPos, vertexNormal, cpuUV);
	return calcuateLight(vertexNormal, objectPos, viewPos, uv, false);
}
