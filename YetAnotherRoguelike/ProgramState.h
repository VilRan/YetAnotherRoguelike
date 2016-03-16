#pragma once

using namespace std;

class Program;
class ProgramState
{
public:
	static const int SelectionMin = 0;
	static const int SelectionMax = 3;
	static const char UpArrow = 'H';
	static const char DownArrow = 'P';
	static const char LeftArrow = 'K';
	static const char RightArrow = 'M';
	static const char NorthWest = 'G';
	static const char NorthEast = 'I';
	static const char SouthWest = 'O';
	static const char SouthEast = 'Q';
	static const char EnterKey = '\r';
	static const char Backspace = '\b';
	static const char EscapeKey = '\033';
	static const char F1Key = ';';


	Program& Program;
	bool IsLocked = false;

	ProgramState(::Program& program);
	virtual ~ProgramState();
	void Initialize();
	bool IsInitialized();
	void Update();

protected:
	virtual void OnInitialize() = 0;
	virtual void OnUpdate(char key, bool isCommand) = 0;
	virtual void OnDraw() = 0;

private:
	bool Initialized = false;

};

