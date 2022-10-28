/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 17:03:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/10/27 18:48:37 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>
#include <opencv2/opencv.hpp>

#include "Solver.hpp"
#include "Renderer.hpp"



int main()
{
	/* Only child process remains now */
	bool error = false;
	try
	{
		GLFWwindow* window = CreateWindow();
		CreateImGuiContext(window);

		Shader shader("res/shaders/basic");
		shader.bind();

		CubeModel cube(shader, FaceletCube());


		RenderingLoop(window, shader, cube);
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		error = true;
	}

	/* Destroy tables used in the solver */
	MoveTables::destroy();
	PruningTables::destroy();

	/* Destroy ImGui context */
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	/* Destroy GLFW */
	glfwTerminate();

	return error ? EXIT_FAILURE : EXIT_SUCCESS;
}
