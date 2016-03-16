#pragma once
#include <string>

using namespace std;

class StringUtility
{
public:
	StringUtility();
	~StringUtility();

	static bool Contains(string fullString, string substring, bool caseSensitive = false);
};

