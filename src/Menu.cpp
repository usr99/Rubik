/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Menu.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 01:12:01 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/20 23:37:23 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Menu.hpp"
#include "Solver.hpp"

Menu::Menu(const std::string& name)
	: name(name), enabled(false) {}

Menu::~Menu() {}

MainMenu::MainMenu(CubeModel& cube) : _Name("Main Menu"), _Cube(cube) {}

MainMenu::~MainMenu()
{
	std::for_each(_Menus.begin(), _Menus.end(), [](Menu* m) { delete m; });
}

void
MainMenu::push(Menu* element)
{
	_Menus.push_back(element);
}

void
MainMenu::render()
{
	/* Render a checkbox list to enable menus */
	ImGui::SetNextWindowSize(ImVec2(180.0f, 169.0f));
	ImGui::Begin(_Name.c_str(), nullptr, ImGuiWindowFlags_NoResize);
	std::for_each(_Menus.begin(), _Menus.end(), [](Menu* m) {
		ImGui::Checkbox(m->name.c_str(), &m->enabled);
	});	
	ImGui::End();

	/* Render all enabled menus */
	std::for_each(_Menus.begin(), _Menus.end(), [this](Menu* m) {
		if (m->enabled)
			m->render(this->_Cube);
	});
}

MovesMenu::MovesMenu() : Menu("Moves"), _Modifiers({ '\0', '2', '\'' }) {}

void
MovesMenu::render(CubeModel& cube)
{
	ImGui::SetNextWindowSize(ImVec2(207.0f, 118.0f));
	ImGui::Begin(name.c_str(), &enabled, ImGuiWindowFlags_NoResize);
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 6; x++)
		{
			if (x > 0)
				ImGui::SameLine();

			if (ImGui::Selectable((std::string(1, Rubik::Faces[x]) + _Modifiers[y]).c_str(), false, 0, ImVec2(25, 25)))
			{
				if (y != 2)
					cube.PushMove(x, 90.0f * (y + 1));
				else
					cube.PushMove(x, -90.0f);
			}
		}
	}
	ImGui::End();
}

AnimationMenu::AnimationMenu() : Menu("Rotation Monitor") {}

void
AnimationMenu::render(CubeModel& cube)
{
	ImGui::SetNextWindowSize(ImVec2(158.0f, 77.0f));
	ImGui::Begin(name.c_str(), &enabled, ImGuiWindowFlags_NoResize);
	ImGui::Checkbox("Toggle animations", &cube.AnimEnabled);
	ImGui::SliderFloat("Speed", &cube.Delay, 0.3f, 5.0f);
	ImGui::End();
}

FaceletColorMenu::FaceletColorMenu(glm::vec3* ColorScheme)
	: Menu("Facelet Color Editor"),
		_Labels{ "Up", "Right", "Front", "Down", "Left", "Back" }
{
	memcpy(_Default, ColorScheme, 6 * sizeof(glm::vec3));
}

void
FaceletColorMenu::render(CubeModel& cube)
{
	ImGui::SetNextWindowSize(ImVec2(207.0f, 207.0f));
	ImGui::Begin(name.c_str(), &enabled, ImGuiWindowFlags_NoResize);

	for (int face = 0; face < 6; face++)
		ImGui::ColorEdit3(_Labels[face].c_str(), &(cube.ColorScheme.data() + face)->r);
	
	float width = ImGui::GetWindowWidth() - ImGui::GetStyle().IndentSpacing * 0.75f;
	ImGui::Spacing();
	if (ImGui::Button("Reset default scheme", ImVec2(width, 30.0f)))
		memcpy(cube.ColorScheme.data(), _Default, 6 * sizeof(glm::vec3));
	
	ImGui::End();
}

ScrambleMenu::ScrambleMenu() : Menu("Scramble"), _ScrambleSize(21), _Input{'\0'} {}

void
ScrambleMenu::render(CubeModel& cube)
{
	ImGui::SetNextWindowSize(ImVec2(359.0f, 100.0f));
	ImGui::Begin(name.c_str(), &enabled, ImGuiWindowFlags_NoResize);

	ImGui::InputText("Sequence", _Input, 200, ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CallbackCharFilter, Filter::FilterSingmasterNotation);
	ImGui::SliderInt("Scramble length", &_ScrambleSize, 1, 50);
	if (ImGui::Button("Generate"))
	{
		std::list<std::string> scramble = generateScramble(_ScrambleSize);
		std::string sequence;

		for (auto m = scramble.begin(); m != scramble.end(); m++)
		{
			sequence.append(*m);
			sequence.push_back(' ');
		}
		size_t len = sequence.copy(_Input, 199);
		_Input[len] = '\0';
	}
	ImGui::SameLine();
	if (ImGui::Button("Apply"))
	{
		char *tmp = _Input;
		try
		{
			cube.ApplySequence(parseScramble(&tmp, 1));
		}
		catch (const std::exception &e)
		{
			ImGui::OpenPopup("Error");
		}
		_Input[0] = '\0';
	}

	/* Modal window showed when the input sequence is invalid */
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("Error", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("The sequence you've entered is not valid.\n");
		if (ImGui::Button("OK"))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
	ImGui::End();
}

SolverMenu::SolverMenu() : Menu("Solver"), _SolverLaunched(false) {}

void
SolverMenu::render(CubeModel& cube)
{
	ImGui::SetNextWindowSize(ImVec2(275.0f, 0.0f));
	ImGui::Begin(name.c_str(), &enabled, ImGuiWindowFlags_AlwaysAutoResize);

	/* Generate solution and save a copy as a string */
	if (_SolverLaunched)
	{
		_Solution = solve(cube.toCubieCube());
		_SolutionStr.clear();
		for (auto move = _Solution.begin(); move != _Solution.end(); move++)
			_SolutionStr += *move + ' ';
		_SolverLaunched = false;
	}

	static ImVec4 labelColor(0.51f, 0.51f, 1.0f, 1.0f);

	/* Print solution if any */
	ImGui::PushTextWrapPos(ImGui::GetWindowWidth() - 5.0f);
	ImGui::TextColored(labelColor, "Solution:\n");
	ImGui::SameLine();
	if (ImGui::Button("Generate"))
	{
		_SolutionStr = "Generating solution... please wait...";
		_SolverLaunched = true;
	}
	if (_Solution.size() || _SolverLaunched)
		ImGui::Text("%s", _SolutionStr.c_str());
	else
		ImGui::Text("(none)");
	
	/* Print moves count if any */
	ImGui::Spacing();
	ImGui::TextColored(labelColor, "Moves:");
	ImGui::SameLine();
	ImGui::Text("%ld", _Solution.size());
    ImGui::PopTextWrapPos();

	/* Solve the cube and clear the text fields */
	if (ImGui::Button("Apply solution"))
	{
		cube.ApplySequence(_Solution);
		_Solution.clear();
		_SolutionStr.clear();
	}

	ImGui::End();
}

FaceletEditorMenu::FaceletEditorMenu() : Menu("Facelet Editor"),
	_NewlineLayout({ 2, 5, 8, 20, 32, 44, 47, 50 }), _SelectedColor(0)
{
	_Init();

	/* Create the layout that describe where each facelet is drawn on the window */
	for (int i = 0; i < 9; i++)
	{
		/* White face is at the top */
		_FaceletsLayout[0 + i] = _Facelets[0 * 9 + i]; 
		/* Orange face is the first in the middle layer */
		_FaceletsLayout[9 + i / 3 * 12 + i % 3] = _Facelets[4 * 9 + i];
		/* Green face is the second in the middle layer */
		_FaceletsLayout[12 + i / 3 * 12 + i % 3] = _Facelets[2 * 9 + i];
		/* Red face is the third in the middle layer */
		_FaceletsLayout[15 + i / 3 * 12 + i % 3] = _Facelets[1 * 9 + i];
		/* Blue face is the fourth in the middle layer */
		_FaceletsLayout[18 + i / 3 * 12 + i % 3] = _Facelets[5 * 9 + i];
		/* Yellow face is at the bottom */
		_FaceletsLayout[45 + i] = _Facelets[3 * 9 + i];
	}	
}

void
FaceletEditorMenu::render(CubeModel& cube)
{
	ImGui::SetNextWindowSize(ImVec2(206.0f, 243.0f));
	ImGui::Begin(name.c_str(), &enabled, ImGuiWindowFlags_NoResize);

	/* Show selected color when the facelets are mouse hovered */
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, {
		cube.ColorScheme[_SelectedColor].x,
		cube.ColorScheme[_SelectedColor].y,
		cube.ColorScheme[_SelectedColor].z,
		1.0f
	});

	int nlIndex = 0;
	ImGui::Indent(9); // center the grid of facelets
	/* Print the 54 facelets based on the layout */
	for (int i = 0; i < 54; i++)
	{
		/* Set facelet color */
		glm::vec3 color = cube.ColorScheme[_Facelets[_FaceletsLayout[i]] / 9];
		ImGui::PushStyleColor(ImGuiCol_Header, {
			color.x,
			color.y,
			color.z,
			1.0f
		});

		/* Indent the up and down faces to form the T-shape */
		if (i == 0 || i == 45)
			ImGui::Indent(45);
		else if (i == 9)
			ImGui::Unindent(45);

		/*
		** Print the facelet, clicking on it set its color to be the currently selected color
		** center facelets cannot be modified
		*/
		std::string id = "##" + std::to_string(i + 1);
		if (ImGui::Selectable(id.c_str(), true, 0, ImVec2(7, 10)) && _FaceletsLayout[i] % 9 != 4)
			_Facelets[_FaceletsLayout[i]] = static_cast<Facelet>(_SelectedColor * 9);
		ImGui::PopStyleColor();

		/*
		** Facelets are printed on the same line by default
		** print newlines only when the "newlines layout" says so
		*/
		ImGui::SameLine();
		if (_NewlineLayout[nlIndex] == i)
		{
			ImGui::NewLine();
			nlIndex++;
		}
	}
	ImGui::PopStyleColor();
	ImGui::Unindent(54);

	/* Color palette */
	ImGui::NewLine();
	ImGui::Separator();
	for (int face = 0; face < 6; face++)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, {
			cube.ColorScheme[face].x,
			cube.ColorScheme[face].y,
			cube.ColorScheme[face].z,
			1.0f
		});

		std::string id = "##" + std::to_string(-face);
		if (ImGui::Button(id.c_str(), { 25.0f, 25.0f }))
			_SelectedColor = face;
		ImGui::PopStyleColor();
		ImGui::SameLine();
	}

	/* Button to apply the colored facelets to the 3D cube */
	ImGui::NewLine();
	ImGui::Separator();
	float width = ImGui::GetWindowWidth() - ImGui::GetStyle().IndentSpacing * 0.75f;
	if (ImGui::Button("Validate", { width, 20.0f }))
	{
		try
		{
			FaceletCube newCube(_Facelets);
			_CheckStateValidity(newCube); // check if the cube is solveable
			cube.setState(newCube);
		}
		catch (const std::exception& e)
		{
			_ErrorMessage = e.what();
			ImGui::OpenPopup("Error");
		}
	}

	if (ImGui::Button("Reset", { width, 20.0f }))
		_Init();

	/* Popup window showed when the cube is not solveable */
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("Error", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("%s\n", _ErrorMessage.c_str());
		if (ImGui::Button("OK"))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}

	ImGui::End();
}

void
FaceletEditorMenu::_Init()
{
	for (unsigned int i = 0; i != _Facelets.size(); i++)
		_Facelets[i] = static_cast<Facelet>(i);
}

void
FaceletEditorMenu::_CheckStateValidity(const FaceletCube& state)
{
	/*
	** Check cubies validity and orientation
	** Validity = not having any cubie in double,
	** no same colors cubies, no opposite colors cubies, etc.
	** Oriention = the sum of the corners orientation must be a multiple of 3
	** and a multiple of 2 for the edges
	*/
	auto corners = state.getCornerCubies();
	auto edges = state.getEdgeCubies();

	/*
	** Check cubies permutation
	** corners permutation and edges permutations parities must be equal,
	** both odd or even
	*/
	int cParity = _GetPermutationParity<CornerCubie, Corner, CORNER_COUNT>(corners);
	int eParity = _GetPermutationParity<EdgeCubie, Edge, EDGE_COUNT>(edges);

	if (cParity % 2 != eParity % 2)
		throw std::runtime_error("Permutation parity is not valid");
}

template <typename T, typename U, unsigned int size>
int	
FaceletEditorMenu::_GetPermutationParity(const std::array<T, size>& cubies)
{
	std::vector<std::vector<U>>				cycles;
	std::array<U, size>						remaining;
	typename std::array<U, size>::iterator	endIterator(remaining.end());

	/* Fill the array with the values of all different cubies */
	for (unsigned int i = 0; i < size; i++)
		remaining[i] = static_cast<U>(i);

	/*
	** Create a vector describing the cycles of permutations
	**
	** We will take the following permutation as an exemple
	** 1 2 3 4 5 6 7 8 ===> 4 2 5 1 6 3 8 7
	**
	** 1 and 4 swapped, this is a 2-cycle
	** 3, 5 and 6 swapped, this is a 3-cycle
	** 7 and 8 swapped, this is another 2-cycle
	** 2 didn't move so it isn't part of any cycle
	**
	** With this example the vector would contains :
	** 1, 2
	** 3, 5, 6
	** 7, 8
	*/
	for (unsigned int i = 0; i < size; i++)
	{
		/*
		** A cubie is not in the right position
		** because in a solved cube, cubie[i] == i
		*/
		if (cubies[i].get() != i && std::find(remaining.begin(), endIterator, i) != endIterator)
		{
			/* Create a new cycle */
			cycles.push_back(std::vector<U>());
			/* Add the first cubie */
			cycles.back().push_back(static_cast<U>(i));
			endIterator = std::remove(remaining.begin(), endIterator, i);

			/* Save the cubie that took its place */
			int next = cubies[i].get();

			/* Add the next cubie until we go back to the cubie that began the cycle */
			while (cubies[next].get() != cubies[i].get())
			{
				cycles.back().push_back(static_cast<U>(next));
				endIterator = std::remove(remaining.begin(), endIterator, next);
				next = cubies[next].get();
			}
		}
	}

	/*
	** Compute the sum of the parities from all the cycles
	** a n-cycle has a parity of n - 1
	*/
	int sum = 0;
	for (auto swap = cycles.begin(); swap != cycles.end(); swap++)
		sum += swap->size() - 1;
	return sum;
}
