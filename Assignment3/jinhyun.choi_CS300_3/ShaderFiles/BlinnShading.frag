#shader fragment

/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: BlinnShading.frag
Purpose: the fragment shader for BlinnShading
Language: GLSL
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/04/2022
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
	vec3 result = calcuateLight(outputNormal, objectPos, cameraPos, uv, true);
	outputColor = vec4(result, 1.0);
}