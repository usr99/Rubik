/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Renderer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 15:52:11 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/16 17:10:58 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <glm/gtc/quaternion.hpp>

#include "stb_image/stb_image.h"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Menu.hpp"
#include "Solver.hpp"

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

void* LoadTables(void* arg)
{
	LoadingInfo* info = (LoadingInfo*)arg;

	try
	{
		MoveTables* mt = MoveTables::getInstance(info);
		PruningTables::getInstance(*mt, info);
	}
	catch (const std::exception& e)
	{
		pthread_mutex_lock(&info->mutex);
		info->error = true;
		pthread_mutex_unlock(&info->mutex);
		return nullptr;
	}
	pthread_mutex_lock(&info->mutex);
	info->done = true;
	pthread_mutex_unlock(&info->mutex);
	return info;
}

bool RenderLoadingScreen(LoadingInfo* state, Camera& camera, CubeModel& cube)
{
	if (pthread_mutex_trylock(&state->mutex) != EBUSY)
	{
		if (state->error)
			throw std::runtime_error("Tables loading failed");

		if (!state->done) // Animate the screen while the tables load
		{
			/* Print filepath currently being loaded */
			ImGui::Begin("Loading", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
			ImGui::Text("%s", state->message.c_str());
			ImVec2 pos(ImGui::GetMainViewport()->Size.x / 2 - ImGui::GetWindowWidth() / 2, ImGui::GetWindowPos().y);
			ImGui::SetWindowPos(pos);
			ImGui::End();

			/* Make a random move on the cube */
			const float angles[] = {-90.0f, 90.0f, 180.0f};
			cube.PushMove(std::rand() % 6, angles[std::rand() % 3]);
		}
		else // Replace the cube at the center of the screen before showing UI
		{
			static double last = glfwGetTime();
			double now = glfwGetTime();
			if (now - last >= 0.01)
			{
				last = now;
				camera.view = glm::translate(camera.view, glm::vec3(0.0f, -0.02f, 0.05f));
				if (camera.view[3].z >= -4.0f)
				{
					cube.setState(FaceletCube());
					pthread_mutex_destroy(&state->mutex);
					return true;
				}
			}
		}
		pthread_mutex_unlock(&state->mutex);
	}
	return false;
}

void RenderingLoop(GLFWwindow* window, Shader& shader, CubeModel& cube)
{
	/* Set ModelViewProjection matrices */
	Camera camera = {
		glm::perspective(glm::radians(45.0f), RATIO(WINDOW_W, WINDOW_H), 0.1f, 50.0f),
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.75f, -6.0f))
	};
	glm::mat4 ModelMatrix(glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(45.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	MainMenu menu(cube);
	menu.push(new ScrambleMenu());
	menu.push(new SolverMenu());
	menu.push(new MovesMenu());
	menu.push(new AnimationMenu());
	menu.push(new FaceletColorMenu(cube.ColorScheme.data()));

	LoadingInfo loadingState;
	pthread_t	loaderth;
	bool		programCanStart = false;

	/* Launch tables loading process into a separate thread */
	loadingState.done = false;
	loadingState.error = false;

	if (pthread_mutex_init(&loadingState.mutex, nullptr) || pthread_create(&loaderth, nullptr, &LoadTables, &loadingState))
		throw std::runtime_error("Couldn't create a new thread");

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		GLCall(glClearColor(0.1f, 0.1, 0.1, 1.0f)); // dark grey
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		/* Update the view */
		shader.bind();
		shader.setUniformMat4f("u_MVP", camera.getMatrix() * ModelMatrix);

		/* Handle mouse inputs for cube controls */
		ImGuiIO& io = ImGui::GetIO();
		if (ImGui::IsMousePosValid() && !ImGui::IsAnyItemActive())
		{
			if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
			{
				RotateModel(ModelMatrix, camera.view, io.MousePos, ImGui::GetMouseDragDelta(ImGuiMouseButton_Left));
				ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
			}
		}

		/* Create the new ImGui frame */
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (!programCanStart)
			programCanStart = RenderLoadingScreen(&loadingState, camera, cube);
		else
			menu.render();
		cube.Render();

		/* Render dear imgui into screen */
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}	
}
