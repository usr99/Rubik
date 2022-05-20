/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Renderer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 15:52:00 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/20 23:37:24 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERER_HPP
# define RENDERER_HPP

# include "imgui/imgui.h"
# include "imgui/imgui_impl_glfw.h"
# include "imgui/imgui_impl_opengl3.h"

# include <GL/glew.h>
# include <GLFW/glfw3.h>

# include <csignal>

# include "Shader.hpp"
# include "CubeModel.hpp"
# include "ATable.hpp"

# define WINDOW_W		800.0f
# define WINDOW_H 		600.0f
# define RATIO(x, y)	(static_cast<float>(x) / static_cast<float>(y))

# define ASSERT(x) if (!(x)) raise(SIGTRAP);
# define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__));

/* OpenGL debugging */
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

struct Camera
{
	inline glm::mat4	getMatrix() const { return projection * view; }

	const glm::mat4		projection;
	glm::mat4			view;
};

GLFWwindow* CreateWindow();
void CreateImGuiContext(GLFWwindow* window);
bool RenderLoadingScreen(LoadingInfo* state, Camera& camera, CubeModel& cube);
void* LoadTables(void* arg);
void RenderingLoop(GLFWwindow* window, Shader& shader, CubeModel& cube);

#endif
