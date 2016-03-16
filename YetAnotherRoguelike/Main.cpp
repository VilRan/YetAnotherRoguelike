#include <time.h>
#include "Program.h"
#include "curses.h"
#include "Graphics.h"

using namespace std;

void main()
{
	srand((unsigned int)time(NULL));

	initscr(); // Start Curses
	Graphics::InitializeColors();
	Program().Run();
	endwin(); // End Curses
}