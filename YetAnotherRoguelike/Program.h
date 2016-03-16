#pragma once
#include "ProgramState.h"
#include "InfoManager.h"

using namespace std;

class Session;
class Program
{
public:
	const char* Version = "v0.1 (Pre-Alpha)";

	Program();
	~Program();

	void Run();
	void Exit();
	InfoManager& GetInfo();
	Session* GetSession();
	void StartNewSession();
	void SaveSession(string name);
	void LoadSession(string name);
	void ChangeState(ProgramState* newState);
	void PushState(ProgramState* newState);
	void PopState();

private:
	InfoManager Info = InfoManager();
	Session* CurrentSession = nullptr;
	//ProgramState* State = nullptr;
	//ProgramState* NewState = nullptr;
	vector<ProgramState*> ActiveStates;
	vector<ProgramState*> DiscardedStates;
	bool Exiting = false;
};

