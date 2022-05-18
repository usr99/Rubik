/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Menu.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 01:12:01 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/18 22:13:53 by mamartin         ###   ########.fr       */
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

	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, {
		cube.ColorScheme[_SelectedColor].x,
		cube.ColorScheme[_SelectedColor].y,
		cube.ColorScheme[_SelectedColor].z,
		1.0f
	});

	int nlIndex = 0;
	ImGui::Indent(9);
	for (int i = 0; i < 54; i++)
	{
		glm::vec3 color = cube.ColorScheme[_Facelets[_FaceletsLayout[i]] / 9];
		ImGui::PushStyleColor(ImGuiCol_Header, {
			color.x,
			color.y,
			color.z,
			1.0f
		});

		if (i == 0 || i == 45)
			ImGui::Indent(45);
		else if (i == 9)
			ImGui::Unindent(45);

		std::string id = "##" + std::to_string(i + 1);
		if (ImGui::Selectable(id.c_str(), true, ImGuiSelectableFlags_AllowItemOverlap, ImVec2(7, 10)) && _FaceletsLayout[i] % 9 != 4)
			_Facelets[_FaceletsLayout[i]] = static_cast<Facelet>(_SelectedColor * 9);
		ImGui::PopStyleColor();

		ImGui::SameLine();

		if (_NewlineLayout[nlIndex] == i)
		{
			ImGui::NewLine();
			nlIndex++;
		}
	}

	ImGui::PopStyleColor();
	ImGui::Unindent(54);
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

	static std::string errMessage;

	ImGui::NewLine();
	ImGui::Separator();
	float width = ImGui::GetWindowWidth() - ImGui::GetStyle().IndentSpacing * 0.75f;
	if (ImGui::Button("Validate", { width, 20.0f }))
	{
		try
		{
			FaceletCube newCube(_Facelets);
			_CheckStateValidity(newCube);
			cube.setState(newCube);
		}
		catch (const std::exception& e)
		{
			errMessage = e.what();
			ImGui::OpenPopup("Error");
		}
	}

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			if (ImGui::BeginPopupModal("Error", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("%s\n", errMessage.c_str());
				if (ImGui::Button("OK"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}


	if (ImGui::Button("Reset", { width, 20.0f }))
		_Init();

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
	std::vector<std::vector<Corner>> cCycles;
	std::vector<Corner> cRemaining({URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB});
	std::vector<Corner>::iterator cEndIterator = cRemaining.end();

	std::vector<std::vector<Edge>> eCycles;
	std::vector<Edge> eRemaining({UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR});
	std::vector<Edge>::iterator eEndIterator = eRemaining.end();

	for (int i = 0; i < CORNER_COUNT; i++)
	{
		if (corners[i].c != i && std::find(cRemaining.begin(), cEndIterator, i) != cEndIterator)
		{
			int next = corners[i].c;

			cCycles.push_back(std::vector<Corner>());
			cCycles.back().push_back(static_cast<Corner>(i));
			cEndIterator = std::remove(cRemaining.begin(), cEndIterator, i);

			while (corners[next].c != corners[i].c)
			{
				cCycles.back().push_back(static_cast<Corner>(next));
				cEndIterator = std::remove(cRemaining.begin(), cEndIterator, next);
				next = corners[next].c;
			}
		}
	}

	for (int i = 0; i < EDGE_COUNT; i++)
	{
		if (edges[i].e != i && std::find(eRemaining.begin(), eEndIterator, i) != eEndIterator)
		{
			int next = edges[i].e;

			eCycles.push_back(std::vector<Edge>());
			eCycles.back().push_back(static_cast<Edge>(i));
			eEndIterator = std::remove(eRemaining.begin(), eEndIterator, i);

			while (edges[next].e != edges[i].e)
			{
				eCycles.back().push_back(static_cast<Edge>(next));
				eEndIterator = std::remove(eRemaining.begin(), eEndIterator, next);
				next = edges[next].e;
			}
		}
	}

	int cParity = 0;
	int eParity = 0;

	for (auto cycle = cCycles.begin(); cycle != cCycles.end(); cycle++)
		cParity += cycle->size() - 1;
	for (auto cycle = eCycles.begin(); cycle != eCycles.end(); cycle++)
		eParity += cycle->size() - 1;
	if (cParity % 2 != eParity % 2)
		throw std::runtime_error("Permutation parity is not valid");
}
