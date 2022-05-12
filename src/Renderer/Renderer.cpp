/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Renderer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 15:52:11 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/13 00:43:40 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <glm/gtc/quaternion.hpp>

#include "stb_image/stb_image.h"
#include "Renderer.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "CubeModel.hpp"

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
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

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

/*
** Get a normalized from the center of the virtual ball O
** to a point P on the virtual ball surface such that P is aligned
** on screen's coordinates
*/
glm::vec3 GetArcballVector(int x, int y)
{
	glm::vec3 P = glm::vec3(
		1.0f * x / WINDOW_W * 2 - 1.0,
		1.0f * y / WINDOW_H * 2 - 1.0,
		0.0f
	);

	P.y = -P.y;
	float OP_Squared = P.x * P.x + P.y * P.y;
	if (OP_Squared <= 1)
		P.z = sqrt(1 - OP_Squared); // Pythagoras
	else
		P = glm::normalize(P); // nearest point
	return P;
}

void RotateModel(glm::mat4& ModelMatrix, const glm::mat4& ViewMatrix, const ImVec2& MousePosition, const ImVec2& MouseDelta)
{
	if (!MouseDelta.x && !MouseDelta.y)
		return ; // the following computations do not work if the last and current mouse positions are the same

	/* Compute vectors describing the initial orientation of the model and the new one */
	glm::vec3 va = GetArcballVector(MousePosition.x - MouseDelta.x, MousePosition.y - MouseDelta.y);
	glm::vec3 vb = GetArcballVector(MousePosition.x, MousePosition.y);

	/* Compute the angle between these two vectors */
	float angle = acos(std::min(1.0f, glm::dot(va, vb)));
	/* Compute the axis of the rotation in camera coordinates */
	glm::vec3 CameraAxis = glm::cross(va, vb);

	/* Compute the matrix allowing us to transpose camera coordinates to object coordinates */
	glm::mat3 CamToObj = glm::inverse(glm::mat3(ViewMatrix) * glm::mat3(ModelMatrix));
	/* Compute the axis of the rotation in object coordinates */
	glm::vec3 ObjAxis = CamToObj * CameraAxis;

	/* Apply the rotation to the ModelMatrix */
	ModelMatrix = glm::rotate(ModelMatrix, angle, ObjAxis);
}

void RenderingLoop(GLFWwindow* window, Shader& shader, CubeModel& cube)
{
	/* Set ModelViewProjection matrices */
	const glm::mat4 projection = glm::perspective(glm::radians(45.0f), RATIO(WINDOW_W, WINDOW_H), 0.1f, 50.0f);
	const glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -8.0f));
	const glm::mat4 CameraMatrix = projection * view;
	glm::mat4 ModelMatrix(1.0f);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		GLCall(glClearColor(0.1f, 0.1, 0.1, 1.0f)); // dark grey
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		/* Update the view */
		shader.bind();
		shader.setUniformMat4f("u_MVP", CameraMatrix * ModelMatrix);

		cube.Render();

		/* Handle mouse inputs for cube controls */
		ImGuiIO& io = ImGui::GetIO();
		if (ImGui::IsMousePosValid() && !ImGui::IsAnyItemActive())
		{
			if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
			{
				RotateModel(ModelMatrix, view, io.MousePos, ImGui::GetMouseDragDelta(ImGuiMouseButton_Left));
				ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
			}
		}

		/* Create the new ImGui frame */
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Moves");
		const char letter[] = { 'U', 'R', 'F', 'D', 'L', 'B' };
		const char modifier[] = { '\0', '2', '\'' };

		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 6; x++)
        	{
        	    if (x > 0)
        	        ImGui::SameLine();

        	    if (ImGui::Selectable((std::string(1, letter[x]) + modifier[y]).c_str(), false, 0, ImVec2(50, 50)))
				{
					if (y != 2)
						cube.PushMove(x, 90.0f * (y + 1));
					else
						cube.PushMove(x, -90.0f);
				}
        	}
		}
		ImGui::End();

		/* Render dear imgui into screen */
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}	
}
