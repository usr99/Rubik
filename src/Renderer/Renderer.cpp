/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Renderer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 15:52:11 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/11 01:27:15 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "Renderer.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "stb_image/stb_image.h"

void WindowResizeCallback(GLFWwindow* window, int w, int h)
{
	(void) window;
	Camera::updateProjection(w, h);
}

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cerr << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << ":" << line << "\n";
		return false;
	}
	return true;
}

GLFWwindow* CreateWindow()
{
	GLFWwindow *window;

	/* Initialize the library */
	if (!glfwInit())
		throw std::runtime_error("GLFW initialization failed");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_W, WINDOW_H, "Rubik 42", NULL, NULL);
	if (!window)
		throw std::runtime_error("Window creation failed");

	/* Set window icon */
	GLFWimage icon;
	icon.pixels = stbi_load("res/images/icon.png", &icon.width, &icon.height, nullptr, 4);
	glfwSetWindowIcon(window, 1, &icon);
	stbi_image_free(icon.pixels);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		throw std::runtime_error("GLEW initialization failed");

	/* Set behavior for transparency and depth buffer comparisons */
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_DEPTH_TEST));

	return window;
}

void CreateImGuiContext(GLFWwindow* window)
{
	/* Setup Dear ImGui context */
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	/* Setup Platform/Renderer bindings */
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	/* Setup Dear ImGui style */
	ImGui::StyleColorsDark();
}

void RenderingLoop(GLFWwindow* window)
{
	Shader shader("res/shaders/basic");
	shader.bind();

	/* Update camera when the window is resized */
	glfwSetWindowSizeCallback(window, &WindowResizeCallback);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		GLCall(glClearColor(0.1f, 0.1, 0.1, 1.0f)); // dark grey
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		/* Update camera view */
		shader.setUniformMat4f("u_Camera", Camera::get());

		/* Create the new ImGui frame */
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		/* Render dear imgui into screen */
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}	
}
