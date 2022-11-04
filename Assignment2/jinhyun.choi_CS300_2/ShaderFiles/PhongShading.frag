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

in vec3 modelPos;
in vec3 objectPos;
in vec3 outputNormal;
in vec2 outputUVCoord;

out vec4 outputColor;

uniform vec3 cameraPos;

void main()
{
	vec2 uv = getTextureCoordinate(modelPos, outputNormal, outputUVCoord);
	vec3 result = calcuateLight(outputNormal, objectPos, cameraPos, uv, false);
	outputColor = vec4(result, 1.0);
}