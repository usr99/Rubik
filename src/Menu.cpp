/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Menu.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 01:12:01 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/15 22:07:27 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
	ImGui::Begin(_Name.c_str());
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
		size_t len = sequence.copy(_Input, 499);
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
