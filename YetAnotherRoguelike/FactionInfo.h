#pragma once
#include <string>
#include <unordered_map>

using namespace std;

class FactionInfo
{
public:
	const unsigned char MaxRelation = 100;

	string ID;
	string Name;

	FactionInfo();
	~FactionInfo();

	unsigned char GetRelation(FactionInfo& other);
	void SetRelation(FactionInfo& other, unsigned char value);
	void SetMutualRelation(FactionInfo& other, unsigned char value);

private:
	unordered_map<string, unsigned char> DefaultRelations;
};

