#shader fragment

/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Solid.frag
Purpose: the fragment shader for basic soild object
Language: GLSL
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

uniform vec3 vertexColor;
out vec4 outputColor;

void main()
{
	outputColor= vec4(vertexColor, 1.0);
}