#shader fragment

/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: PhongLighting.frag
Purpose: The fragment shader to draw 3D model of PhongLighting
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 10/24/2022
End Header --------------------------------------------------------*/

in vec3 resultColor;
out vec4 outputColor;

void main()
{
	vec3 result = resultColor;
	outputColor = vec4(result, 1.0);
}