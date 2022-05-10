/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 17:03:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/11 01:57:16 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>
#include <csignal>

#include "Solver.hpp"
#include "Renderer.hpp"

int main()
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
		GLFWwindow* window = CreateWindow();
		CreateImGuiContext(window);
		RenderingLoop(window);
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
