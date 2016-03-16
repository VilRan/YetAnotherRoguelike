#include "CharacterCreationState.h"
#include "Program.h"
#include "curses.h"
#include "Graphics.h"
#include "MapState.h"
#include "MainMenuState.h"
#include "Session.h"

CharacterCreationState::CharacterCreationState(::Program& program) : ProgramState(program)
{
}

CharacterCreationState::~CharacterCreationState()
{
}

void CharacterCreationState::OnInitialize()
{
	//Draw();
}

void CharacterCreationState::OnUpdate(char key, bool isCommand)
{
	Input(key, isCommand);
	//Draw();
}

void CharacterCreationState::Input(char key, bool isCommand)
{
	if (key >= 'A' && key <= 'z')
	{
		Name += key;
	}
	else
	{
		switch (key)
		{
		case ' ':
		case '-':
		case '\'':
			Name += key;
			break;
		case Backspace:
			if (Name.size() > 0)
				Name.pop_back();
			break;
		case EnterKey:
			if (Name.size() > 0)
			{
				Program.StartNewSession();
				Program.GetSession()->Name = Name;
				Program.ChangeState(new MapState(Program));
			}
			break;
		case EscapeKey:
			Program.ChangeState(new MainMenuState(Program));
			break;
		default:
			break;
		}
	}
}

void CharacterCreationState::OnDraw()
{
	Graphics::Print("Name: " + Name, 1, 1, Left);
}
