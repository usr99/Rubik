/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 17:03:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/14 03:29:03 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>

#include "Solver.hpp"
#include "Renderer.hpp"

int main(int ac, char **av)
{
	/* Detach the process from the shell that executed it */
	pid_t pid = fork();
	if (pid < 0)
		return (EXIT_FAILURE); // fork() failed
	else if (pid)
		return (EXIT_SUCCESS); // exit the parent process

	/* Only child process remains now */
	bool error = false;
	try
	{
		std::list<std::string> scramble = parseScramble(av + 1, ac - 1);

		GLFWwindow* window = CreateWindow();
		CreateImGuiContext(window);

		Shader shader("res/shaders/basic");
		shader.bind();

		CubeModel cube(shader, CubieCube(scramble).toFacelet());
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
