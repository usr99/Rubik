/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 17:03:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/12 19:21:17 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>
#include <csignal>

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
		/* Parse scramble from arguments */
		std::list<std::string> scramble;
		for (int i = 1; i < ac; i++)
		{
			std::istringstream iss(av[i]);
			std::string buf;

			while (std::getline(iss, buf, ' '))
			{
				if (buf.size() != 0)
					scramble.push_back(buf);
			}
		}
		if (!scramble.size())
			throw std::invalid_argument("No scramble provided");

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
