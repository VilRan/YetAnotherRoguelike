#include "MainMenuState.h"
#include "curses.h"
#include "Session.h"
#include "MenuItem.h"
#include "MapState.h"
#include "Program.h"
#include "Graphics.h"
#include "Menu.h"
#include "LoadGameState.h"
#include "CharacterCreationState.h"

MainMenuState::MainMenuState(::Program& program) 
	: ProgramState::ProgramState(program)
{
	MainMenu = new Menu();
}


MainMenuState::~MainMenuState()
{
	delete MainMenu;
	/*
	for each (MenuItem* item in Items)
	{
		delete item;
	}
	*/
}

void MainMenuState::OnInitialize()
{
	Session* session = Program.GetSession();
	if (session != nullptr)
	{
		MainMenu->AddItem(new ResumeGameMenuItem("Resume", Program));
		if (session->Player != nullptr)
			MainMenu->AddItem(new SaveGameMenuItem("Save", Program));
	}
	MainMenu->AddItem(new NewGameMenuItem("New", Program));
	MainMenu->AddItem(new LoadGameMenuItem("Load", Program));
	MainMenu->AddItem(new ExitGameMenuItem("Exit", Program));

	//Draw();
}

void MainMenuState::OnUpdate(char key, bool isCommand)
{
	Input(key, isCommand);
	//Draw();
}

void MainMenuState::Input(char key, bool isCommand)
{
	switch (key)
	{
	case EscapeKey:
		if (Program.GetSession() != nullptr)
			Program.ChangeState(new MapState(Program));
		break;
	default:
		MainMenu->Input(key, isCommand);
		break;
	}
	/*
	int min = 0;
	int max = Items.size() - 1;
	if (Selection < min)
		Selection = max;
	else if (Selection > max)
		Selection = min;
	*/
}

void MainMenuState::OnDraw()
{
	int rows, cols;
	getmaxyx(stdscr, rows, cols);
	Graphics::DrawBox(0, 0, cols, 3);

	int row = 1, col = cols / 2;
	Graphics::UseColor(COLOR_WHITE + 8, COLOR_BLACK);
	Graphics::Print("- Yet Another Roguelike -", col, row, Center);
	Graphics::UseColor(COLOR_WHITE, COLOR_BLACK);
	Graphics::Print(Program.Version, cols - 2, row, Right);
#if _DEBUG
	Graphics::Print("DEBUG MODE", 2, row);
#endif

	row = 6;
	col -= 2;
	Graphics::DrawBox(col - 4, row - 2, 13, 10);
	Graphics::UseColor(COLOR_WHITE + 8, COLOR_BLACK);
	MainMenu->Draw(col, row, 12, 8);
	/*
	for (int i = 0; i < MainMenu->GetCount(); i++)
	{
		MenuItem* item = MainMenu->GetItem(i);
		mvprintw(row++, col, "%s", item->Label.data());
	}
	mvprintw(6 + MainMenu->GetSelection(), col - 2, "->");
	*/

	Graphics::UseColor(COLOR_WHITE, COLOR_BLACK);
	Graphics::Print("Press F1 at any time to view the manual.", 2, 23);
	Graphics::Print("Made by Ville Rantala", cols - 2, 23, Right);
	move(24, 0);
}

NewGameMenuItem::NewGameMenuItem(string label, ::Program& program) : MenuItem(label), Program(program)
{
}
NewGameMenuItem::~NewGameMenuItem()
{
}
void NewGameMenuItem::Select()
{
	//Program.StartNewSession();
	//Program.ChangeState(new MapState(Program));
	Program.ChangeState(new CharacterCreationState(Program));
}


ExitGameMenuItem::ExitGameMenuItem(string label, ::Program& program) : MenuItem(label), Program(program)
{
}
ExitGameMenuItem::~ExitGameMenuItem()
{
}
void ExitGameMenuItem::Select()
{
	Program.Exit();
}


ResumeGameMenuItem::ResumeGameMenuItem(string label, ::Program& program) : MenuItem(label), Program(program)
{
}
ResumeGameMenuItem::~ResumeGameMenuItem()
{
}
void ResumeGameMenuItem::Select()
{
	if (Program.GetSession() != nullptr)
		Program.ChangeState(new MapState(Program));
}


SaveGameMenuItem::SaveGameMenuItem(string label, ::Program& program) : MenuItem(label), Program(program)
{
}
SaveGameMenuItem::~SaveGameMenuItem()
{
}
void SaveGameMenuItem::Select()
{
	Program.SaveSession("Saves/" + Program.GetSession()->Name);
}


LoadGameMenuItem::LoadGameMenuItem(string label, ::Program& program) : MenuItem(label), Program(program)
{
}
LoadGameMenuItem::~LoadGameMenuItem()
{
}
void LoadGameMenuItem::Select()
{
	//Program.LoadSession("test");
	Program.ChangeState(new LoadGameState(Program, "./Saves/"));
}