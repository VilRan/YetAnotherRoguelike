#include "Graphics.h"
#include "curses.h"


Graphics::Graphics()
{
}


Graphics::~Graphics()
{
}

void Graphics::DrawBox(int x, int y, int width, int height)
{
	for (int i = 0; i < height; i++)
		mvaddch(y + i, x, '|');

	int x2 = x + width - 1;
	for (int i = 0; i < height; i++)
		mvaddch(y + i, x2, '|');

	move(y, x);
	for (int i = 0; i < width; i++)
		addch('-');

	move(y + height - 1, x);
	for (int i = 0; i < width; i++)
		addch('-');

	mvaddch(y, x, '/');
	mvaddch(y + height - 1, x, '\\');
	mvaddch(y, x + width - 1, '\\');
	mvaddch(y + height - 1, x + width - 1, '/');
}

void Graphics::DrawBox(int x, int y, int width, int height, char c)
{
	move(y, x);
	for (int i = 0; i < width; i++)
		addch(c);

	move(y + height - 1, x);
	for (int i = 0; i < width; i++)
		addch(c);

	for (int i = 0; i < height; i++)
		mvaddch(y + i, x, c);

	int x2 = x + width - 1;
	for (int i = 0; i < height; i++)
		mvaddch(y + i, x2, c);
}


void Graphics::Print(const char * string, int x, int y, TextAlign alignment)
{
	switch (alignment)
	{
	case Left:
		mvprintw(y, x, string);
		break;
	case Center:
		mvprintw(y, x - strlen(string) / 2, string);
		break;
	case Right:
		mvprintw(y, x - strlen(string), string);
		break;
	default:
		break;
	}
}

void Graphics::Print(string str, int x, int y, TextAlign alignment)
{
	Graphics::Print(str.data(), x, y, alignment);
}

void Graphics::MoveCursor(int x, int y)
{
	move(y, x);
}

void Graphics::UseColor(int foreground, int background)
{
	int i = 1 + foreground + background * COLORS;
	attron(COLOR_PAIR(i));
}

void Graphics::InitializeColors()
{
	start_color(); // Enables colors for Curses

	int i = 1; // i = 0 is reserved.
	for (int background = 0; background < COLORS; background++)
		for (int foreground = 0; foreground < COLORS; foreground++)
		{
			init_pair(i, foreground, background);
			i++;
		}
}
