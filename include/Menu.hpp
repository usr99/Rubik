/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Menu.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 00:52:57 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/19 00:42:05 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MENU_HPP
# define MENU_HPP

# include <string>
# include <vector>
# include <memory>

# include "CubeModel.hpp"

class Menu
{
	public:

		Menu(const std::string& name);
		virtual ~Menu();

		virtual void render(CubeModel& cube) = 0;

		const std::string name;
		bool enabled;
};

class MainMenu
{
	public:

		MainMenu(CubeModel &cube);
		~MainMenu();

		void push(Menu *element);
		void render();

	private:

		const std::string	_Name;
		std::vector<Menu*>	_Menus;
		CubeModel&			_Cube;
};

class MovesMenu : public Menu
{
	public:

		MovesMenu();
		virtual void render(CubeModel& cube);

	private:

		std::array<char, 3> _Modifiers;
};

class AnimationMenu : public Menu
{
	public:

		AnimationMenu();
		virtual void render(CubeModel& cube);
};

class FaceletColorMenu : public Menu
{
	public:

		FaceletColorMenu(glm::vec3* ColorScheme);
		virtual void render(CubeModel& cube);

	private:

		glm::vec3					_Default[6];
		std::array<std::string, 6>	_Labels;
};

class ScrambleMenu : public Menu
{
	public:

		ScrambleMenu();
		virtual void render(CubeModel& cube);

	private:

		struct Filter
		{
			// Return 0 (pass) if the character is found in the string " frubldFRUBLD2'"
			static int FilterSingmasterNotation(ImGuiInputTextCallbackData *data)
			{
				if (data->EventChar < 256 && strchr(" frubldFRUBLD2\'", (char)data->EventChar))
					return 0;
				return 1;
			}
		};

		int	 _ScrambleSize;
		char _Input[200];
};

class SolverMenu : public Menu
{
	public:

		SolverMenu();
		virtual void render(CubeModel& cube);

	private:

		bool					_SolverLaunched;
		std::list<std::string>	_Solution;
		std::string				_SolutionStr;
};

class FaceletEditorMenu : public Menu
{
	public:

		FaceletEditorMenu();
		virtual void render(CubeModel& cube);

	private:

		void	_Init();
		void	_CheckStateValidity(const FaceletCube& state);

		template <typename T, typename U, unsigned int size>
		int		_GetPermutationParity(const std::array<T, size>& cubies);

		std::array<Facelet, 54>	_Facelets;
		std::array<Facelet, 54>	_FaceletsLayout;
		std::array<int, 8>		_NewlineLayout;
		int						_SelectedColor;
		std::string				_ErrorMessage;
};

#endif
