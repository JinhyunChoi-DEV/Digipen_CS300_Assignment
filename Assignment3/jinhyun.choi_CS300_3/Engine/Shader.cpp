/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Shader.cpp
Purpose: Making shader for doing some action like SetUniform, UseProgram, and other this.
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include <iostream>
#include <glad/glad.h>
#include "Shader.hpp"

#include "ShaderManager.hpp"

namespace
{
	GLuint CompileShader(std::string name, std::string versionCode, std::string shaderBaseCode, std::vector<std::string> commonCode, GLenum shaderType);
	GLuint LinkShader(GLuint vertex_handle, GLuint fragment_handle);
	char* StringCastToChar(std::string text);
}

Shader::Shader(std::string name, std::vector<std::pair<ShaderType, std::string>> shaderInfos)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::vector<std::string> commonCode;

	for (auto shaderInfo : shaderInfos)
	{
		auto type = shaderInfo.first;

		if (type == Vertex)
			vertexCode = shaderInfo.second;

		if (type == Fragment)
			fragmentCode = shaderInfo.second;

		if (type == GLSLCommon)
			commonCode.push_back(shaderInfo.second);
	}

	unsigned vertex = CompileShader(name, versionText, vertexCode, commonCode, GL_VERTEX_SHADER);
	unsigned fragment = CompileShader(name, versionText, fragmentCode, commonCode, GL_FRAGMENT_SHADER);

	ProgramID = LinkShader(vertex, fragment);
	Name = name;

	glDeleteProgram(vertex);
	glDeleteProgram(fragment);
}

void Shader::Set(const std::string& name, bool value) const
{
	const GLint location = glGetUniformLocation(ProgramID, name.c_str());

	if (location < 0)
	{
		std::cout << "Error-Shader: Fail To Get UniformLocation => " << name << std::endl;
		return;
	}

	glUniform1i(location, static_cast<int>(value));
}

void Shader::Use()
{
	glUseProgram(ProgramID);
}

void Shader::Set(const std::string& name, int value) const
{
	const GLint location = glGetUniformLocation(ProgramID, name.c_str());

	if (location < 0)
	{
		std::cout << "Error-Shader: Fail To Get UniformLocation - " << Name << "=> " << name << std::endl;
		return;
	}

	glUniform1i(location, value);
}

void Shader::Set(const std::string& name, unsigned int value) const
{
	const GLint location = glGetUniformLocation(ProgramID, name.c_str());

	if (location < 0)
	{
		std::cout << "Error-Shader: Fail To Get UniformLocation - " << Name << "=> " << name << std::endl;
		return;
	}

	glUniform1ui(location, value);
}

void Shader::Set(const std::string& name, float value) const
{
	const GLint location = glGetUniformLocation(ProgramID, name.c_str());

	if (location < 0)
	{
		std::cout << "Error-Shader: Fail To Get UniformLocation - " << Name << "=> " << name << std::endl;
		return;
	}

	glUniform1f(location, value);
}

void Shader::Set(const std::string& name, glm::vec3 value) const
{
	const GLint location = glGetUniformLocation(ProgramID, name.c_str());

	if (location < 0)
	{
		std::cout << "Error-Shader: Fail To Get UniformLocation - " << Name << "=> " << name << std::endl;
		return;
	}

	glUniform3fv(location, 1, &value[0]);
}

void Shader::Set(const std::string& name, glm::vec4 value) const
{
	const GLint location = glGetUniformLocation(ProgramID, name.c_str());

	if (location < 0)
	{
		std::cout << "Error-Shader: Fail To Get UniformLocation - " << Name << "=> " << name << std::endl;
		return;
	}

	glUniform4fv(location, 1, &value[0]);
}

void Shader::Set(const std::string& name, glm::mat4 value) const
{
	const GLint location = glGetUniformLocation(ProgramID, name.c_str());

	if (location < 0)
	{
		std::cout << "Error-Shader: Fail To Get UniformLocation - " << Name << "=> " << name << std::endl;
		return;
	}

	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

namespace
{
	GLuint CompileShader(std::string name, std::string versionCode, std::string shaderBaseCode, std::vector<std::string> commonCode, GLenum shaderType)
	{
		GLuint shader = 0;
		GLint compile_result = GL_FALSE;
		int commonCodeCount = (int)commonCode.size();
		int totalSize = commonCodeCount + 2;

		char** codes = new char* [totalSize];
		codes[0] = StringCastToChar(versionCode);
		for (int i = 0; i < (int)commonCode.size(); ++i)
		{
			char* currentCode = StringCastToChar(commonCode[i]);
			codes[i + 1] = currentCode;
		}
		codes[totalSize - 1] = StringCastToChar(shaderBaseCode);

		shader = glCreateShader(shaderType);

		glShaderSource(shader, totalSize, codes, nullptr);

		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

		if (compile_result == GL_FALSE)
		{
			std::string error_log = "Failed to compile: " + name;
			if (shaderType == GL_FRAGMENT_SHADER)
				error_log += " fragment shader:\n";
			else if (shaderType == GL_VERTEX_SHADER)
				error_log += " vertex shader:\n";
			GLint log_length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

			GLchar* log_message = new GLchar[log_length];
			glGetShaderInfoLog(shader, log_length, nullptr, log_message);

			std::cerr << error_log << log_message << "\n";
			assert(false);
			delete[] log_message;
		}
		return shader;
	}

	GLuint LinkShader(GLuint vertex_handle, GLuint fragment_handle)
	{
		GLint link_result = GL_FALSE;
		GLuint program = 0;
		program = glCreateProgram();

		glAttachShader(program, vertex_handle);
		glAttachShader(program, fragment_handle);

		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &link_result);

		if (link_result == GL_FALSE)
		{
			GLint log_length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

			GLchar* log_message = new GLchar[log_length];
			glGetProgramInfoLog(program, log_length, nullptr, log_message);

			std::cerr << "shader program failed to link\n" << log_message << "\n";
			assert(false);
			delete[] log_message;
		}
		return program;
	}

	char* StringCastToChar(std::string text)
	{
		const char* textData = text.c_str();
		int size = (int)(strlen(textData) + 1);
		char* result = new char[size];
		strcpy_s(result, size, textData);

		return result;
	}
}
