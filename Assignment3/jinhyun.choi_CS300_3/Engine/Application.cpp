/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Application.cpp
Purpose: Making of application for opening window, inputs, and many other application level api
Language: C++
Platform: Windows 11
Project: jinhyun.choi_CS300_3
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 9/29/2022
End Header --------------------------------------------------------*/

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Application.hpp"
#include "Input.hpp"
#include "Graphic.hpp"

void error_callback(int error_code, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double x, double y);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);

Application* APPLICATION;

Application::Application()
{
	APPLICATION = this;
	windowSize.x = 1600;
	windowSize.y = 900;

	glfwSetErrorCallback(error_callback);

	const int success = glfwInit();

	if (success == GLFW_FALSE)
	{
		std::cout << "Error-Application: Fail to Initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y), "CS300_JinhyunChoi", NULL, NULL);
	if (window == nullptr)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Error-Application: Fail to Initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}

	gui = new GUIManager(window);
}

Application::~Application()
{
	delete gui;
	gui = nullptr;
}

void Application::Initialize()
{
	gui->Initialize();
	GRAPHIC->SetViewSize(windowSize);
}

void Application::Update()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Application::Terminate()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool Application::IsWindowClose() const
{
	return glfwWindowShouldClose(window);
}

double Application::GetGameTime() const
{
	return glfwGetTime();
}

void Application::SetWindowSize(const int width, const int height)
{
	windowSize.x = static_cast<float>(width);
	windowSize.y = static_cast<float>(height);

	glfwSetWindowSize(window, width, height);
}

void error_callback(int error_code, const char* description)
{
	fprintf(stderr, "Error: %s \n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	INPUT->SetKeyboard(key, action);
}

void mouse_callback(GLFWwindow* window, double x, double y)
{
	INPUT->SetMouse(x, y);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	APPLICATION->SetWindowSize(width, height);
	GRAPHIC->SetViewSize(APPLICATION->GetWindowSize());
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	INPUT->SetMouseButton(button, action);
}
