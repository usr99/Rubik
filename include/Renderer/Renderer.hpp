/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Renderer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 15:52:00 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/11 01:28:07 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERER_HPP
# define RENDERER_HPP

# include "imgui.h"
# include "imgui_impl_glfw.h"
# include "imgui_impl_opengl3.h"

# include <GL/glew.h>
# include <GLFW/glfw3.h>

# include <csignal>

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

GLFWwindow* CreateWindow();
void CreateImGuiContext(GLFWwindow* window);
void RenderingLoop(GLFWwindow* window);

#endif
