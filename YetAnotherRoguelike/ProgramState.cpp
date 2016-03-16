#pragma once
#include "ProgramState.h"
#include <conio.h>
#include "curses.h"
#include "Program.h"
#include "ManualState.h"


ProgramState::ProgramState(::Program& program) : Program(program)
{
}

ProgramState::~ProgramState()
{
}

void ProgramState::Initialize()
{
	clear();
	OnInitialize();
	refresh();
	Initialized = true;
}

bool ProgramState::IsInitialized()
{
	return Initialized;
}

void ProgramState::Update()
{
	clear();
	OnDraw();
	refresh();

	char key = 0; 
	bool isCommand = false;
	if (IsLocked)
	{
		key = 0;
	}
	else
	{
		key = _getch();
		if (key == -32 || key == 0)
		{
			isCommand = true;
			key = _getch();
		}
	}

	bool doUpdate = true;
	//clear();
	if (isCommand)
	{
		switch (key)
		{
		case F1Key:
			Program.PushState(new ManualState(Program));
			doUpdate = false;
			break;
		}
	}
	
	if (doUpdate)
		OnUpdate(key, isCommand);
	//refresh();
}
