#include "StringUtility.h"
#include <algorithm>

StringUtility::StringUtility()
{
}


StringUtility::~StringUtility()
{
}

bool StringUtility::Contains(string fullString, string substring, bool caseSensitive)
{
	if (!caseSensitive)
	{
		transform(fullString.begin(), fullString.end(), fullString.begin(), ::tolower);
		transform(substring.begin(), substring.end(), substring.begin(), ::tolower);
	}

	if (fullString.find(substring) == string::npos)
		return false;

	return true;
}
