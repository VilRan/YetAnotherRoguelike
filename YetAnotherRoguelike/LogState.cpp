#include "LogState.h"
#include "curses.h"
#include "Program.h"
#include "MapState.h"
#include "Session.h"
#include <algorithm>
#include "Graphics.h"
#include "StringUtility.h"

LogState::LogState(::Program& program)
	: ProgramState::ProgramState(program)
{
}


LogState::~LogState()
{
}

void LogState::OnInitialize()
{
	//Draw();
}

void LogState::OnUpdate(char key, bool isCommand)
{
	Input(key, isCommand);
	//Draw();
}

void LogState::Input(char key, bool isCommand)
{
	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	if (isCommand)
	{
		switch (key)
		{
		case '\0':
			break;
		case UpArrow:
			Offset--;
			break;
		case DownArrow:
			Offset++;
			break;
		case NorthEast:
			Offset -= rows;
			break;
		case SouthEast:
			Offset += rows;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (key)
		{
		case '\0':
			break;
		case EscapeKey:
			Program.ChangeState(new MapState(Program));
			break;
		case Backspace:
			if (Filter.size() > 0)
				Filter.pop_back();
			break;
		case EnterKey:
			break;
		default:
			Filter += key;
			break;
		}
	}

	Session& session = *Program.GetSession();
	int max = session.Log.size() - 1;
	if (Offset >= max)
		Offset = max;
	else if (Offset < 0)
		Offset = 0;
}

void LogState::OnDraw()
{
	Session& session = *Program.GetSession();
	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	Graphics::UseColor(COLOR_WHITE, COLOR_BLACK);
	int i = -1;
	int max = rows + Offset - 1;
	for (string entry : session.Log)
	{
		/*
		string entryLC = entry; // Lower case copy of the entry for case insensitive filtering.
		transform(entryLC.begin(), entryLC.end(), entryLC.begin(), ::tolower);
		string filterLC = Filter; // Same thing for the filter itself.
		transform(filterLC.begin(), filterLC.end(), filterLC.begin(), ::tolower);
		
		if (entryLC.find(filterLC) == string::npos)
			continue;
		*/
		
		if ( ! StringUtility::Contains(entry, Filter, false))
			continue;

		i++;
		if (i < Offset)
			continue;
		if (i >= max)
			break;

		if (i == 0)
			Graphics::UseColor(COLOR_WHITE + 8, COLOR_BLACK);
		else
			Graphics::UseColor(COLOR_WHITE, COLOR_BLACK);

		printw("%s\n", entry.data());
	}

	Graphics::UseColor(COLOR_WHITE, COLOR_BLACK);
	mvprintw(rows - 1, 0, "Filter String: %s", Filter.data());
}