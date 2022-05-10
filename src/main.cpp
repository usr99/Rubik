/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 17:03:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/10 19:06:26 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>
#include <csignal>

#include "Solver.hpp"
#include "Renderer.hpp"

void sig_handler(int sig)
{
	std::cout	<< "Received signal " << sig
				<< "\nTerminating...\n";

	MoveTables::destroy();
	PruningTables::destroy();

	exit(EXIT_FAILURE);
}

int main()
{
	bool error = false;

	try
	{
		/* Set signals handler */
		if (signal(SIGINT, sig_handler) == SIG_ERR ||
			signal(SIGTERM, sig_handler) == SIG_ERR)
				throw std::runtime_error("Setting signal handlers failed");

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

	return error ? 1 : 0;
}
