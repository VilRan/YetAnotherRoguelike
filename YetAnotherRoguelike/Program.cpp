#include "Program.h"
#include <conio.h>
#include "curses.h"
#include "MainMenuState.h"
#include "Session.h"


Program::Program()
{
	//State = new MainMenuState(*this);
	//State->Initialize();
	PushState(new MainMenuState(*this));
}

Program::~Program()
{
	//delete State;
	for (ProgramState* state : ActiveStates)
	{
		delete state;
	}
	
	for (ProgramState* discarded : DiscardedStates)
	{
		delete discarded;
	}

	delete CurrentSession;
}

void Program::Run()
{
	while (!Exiting)
	{
		for (ProgramState* discarded :DiscardedStates)
			delete discarded;
		DiscardedStates.clear();

		if (ActiveStates.size() == 0)
			break;
		ProgramState* state = ActiveStates.back();
		if (!state->IsInitialized())
			state->Initialize();
		state->Update();

		/*
		if (NewState != nullptr)
		{
			delete State;
			State = NewState;
			State->Initialize();
			NewState = nullptr;
		}

		State->Update();
		*/
	}
}

void Program::Exit()
{
	Exiting = true;
}

InfoManager & Program::GetInfo()
{
	return Info;
}

Session * Program::GetSession()
{
	return CurrentSession;
}

void Program::StartNewSession()
{
	if (CurrentSession != nullptr)
		delete CurrentSession;
	CurrentSession = new Session(*this);
	CurrentSession->Initialize();
}

void Program::SaveSession(string name)
{
	CurrentSession->SaveToFile(name);
}

void Program::LoadSession(string name)
{
	if (CurrentSession != nullptr)
		delete CurrentSession;
	CurrentSession = new Session(*this);
	CurrentSession->Initialize(name);
}

void Program::ChangeState(ProgramState * newState)
{
	//NewState = newState;
	PopState();
	PushState(newState);
}

void Program::PushState(ProgramState * newState)
{
	ActiveStates.push_back(newState);
}

void Program::PopState()
{
	DiscardedStates.push_back(ActiveStates.back());
	ActiveStates.pop_back();
}