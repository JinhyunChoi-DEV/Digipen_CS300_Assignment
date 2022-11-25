#shader vertex

/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: FrameBuffer.vert
Purpose: the vertex shader for FrameBuffer
Language: GLSL
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/23/2022
End Header --------------------------------------------------------*/

layout(location=0) in vec3 position;

out vec3 objectPos;

void main()
{
	objectPos = position;

	gl_Position = getPosition(position);
}