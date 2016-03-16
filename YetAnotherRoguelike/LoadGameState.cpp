#include "LoadGameState.h"
#include "tinydir.h"
#include "Menu.h"
#include "Program.h"
#include "MapState.h"
#include "MainMenuState.h"
#include "StringUtility.h"
#include "Graphics.h"

LoadGameState::LoadGameState(::Program& program, string directory)
	: ProgramState(program), Directory(directory), Files(new Menu())
{
	tinydir_dir dir;
	tinydir_open(&dir, Directory.data());

	while (dir.has_next)
	{
		tinydir_file file;
		tinydir_readfile(&dir, &file);

		if (file.is_dir)
		{
			
			if ((string)file.name == ".")
				Files->AddItem(new LoadDirMenuItem((string)file.name, Program, (string)file.name + "/"));
			else if ((string)file.name == "..")
				Files->AddItem(new LoadDirMenuItem((string)file.name, Program, Directory + file.name + "/"));
			else
				Files->AddItem(new LoadDirMenuItem((string)file.name + "/", Program, Directory + file.name + "/"));
			
		}
		else if (StringUtility::Contains(file.name, ".save"))
			Files->AddItem(new LoadFileMenuItem(file.name, Program, Directory + file.name));

		tinydir_next(&dir);
	}

	tinydir_close(&dir);
}


LoadGameState::~LoadGameState()
{
	delete Files;
}

void LoadGameState::OnInitialize()
{
}

void LoadGameState::OnUpdate(char key, bool isCommand)
{
	switch (key)
	{
	case EscapeKey:
		Program.ChangeState(new MainMenuState(Program));
		break;
	default:
		Files->Input(key, isCommand);
		break;
	}
}

void LoadGameState::OnDraw()
{
	Graphics::Print("Path: " + Directory, 1, 1);
	Files->Draw(3, 3, 40, 20);
	Graphics::MoveCursor(0, 24);
}

LoadFileMenuItem::LoadFileMenuItem(string label, ::Program& program, string fullDirectory)
	: MenuItem(label), Program(program), FullDirectory(fullDirectory)
{
}

LoadFileMenuItem::~LoadFileMenuItem()
{
}

void LoadFileMenuItem::Select()
{
	Program.LoadSession(FullDirectory);
	Program.ChangeState(new MapState(Program));
}

LoadDirMenuItem::LoadDirMenuItem(string label, ::Program & program, string fullDirectory)
	: MenuItem(label), Program(program), FullDirectory(fullDirectory)
{

}

LoadDirMenuItem::~LoadDirMenuItem()
{

}

void LoadDirMenuItem::Select()
{
	Program.ChangeState(new LoadGameState(Program, FullDirectory));
}
