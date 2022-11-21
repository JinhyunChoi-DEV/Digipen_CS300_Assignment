#shader fragment

/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Skybox.frag
Purpose: the fragment shader for Skybox
Language: GLSL
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 11/20/2022
End Header --------------------------------------------------------*/

uniform sampler2D top;
uniform sampler2D bottom;
uniform sampler2D right;
uniform sampler2D left;
uniform sampler2D back;
uniform sampler2D front;

in vec3 objectPos;
out vec4 outputColor;

void main()
{
	vec2 uv = vec2(0);
	vec3 absVector = abs(objectPos);
	vec4 resultTexture;

	if(absVector.x >= absVector.y && absVector.x >= absVector.z)
	{
		if(objectPos.x < 0.0f)
			uv.x = -objectPos.z/absVector.x;
		else
			uv.x =  objectPos.z/absVector.x;

		uv.y = objectPos.y/absVector.x;
		uv = (uv+vec2(1))/2.0;

		if(objectPos.x > 0)
			resultTexture = texture(left, uv);
		else
			resultTexture = texture(right, uv);
	}
	else if(absVector.y >= absVector.x && absVector.y >= absVector.z)
	{
		if(objectPos.y < 0.0f)
			uv.y = objectPos.z/absVector.y;
		else
			uv.y = -objectPos.z/absVector.y;

		uv.x = -objectPos.x/absVector.y;
		uv = (uv+vec2(1))/2.0;
 
		if(objectPos.y > 0)
			resultTexture = texture(top, uv);
		else
			resultTexture = texture(bottom, uv);
	}
	else if(absVector.z >= absVector.x && absVector.z >= absVector.y)
	{
		if(objectPos.z > 0.0f)
			uv.x = -objectPos.x/absVector.z;
		else
			uv.x = objectPos.x/absVector.z;

		uv.y = objectPos.y/absVector.z;
		uv = (uv+vec2(1))/2.0;

		if(objectPos.z > 0)
			resultTexture = texture(front, uv);
		else
			resultTexture = texture(back, uv);
	}

	outputColor = resultTexture;
}