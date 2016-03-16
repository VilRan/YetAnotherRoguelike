#include "FactionInfo.h"



FactionInfo::FactionInfo()
{
}


FactionInfo::~FactionInfo()
{
}

unsigned char FactionInfo::GetRelation(FactionInfo & other)
{
	if (this == &other)
		return MaxRelation;

	unordered_map<string, unsigned char>::const_iterator iterator = DefaultRelations.find(other.ID);

	if (iterator == DefaultRelations.end())
		return 0;
	else
		return iterator->second;
}

void FactionInfo::SetRelation(FactionInfo & other, unsigned char value)
{
	if (this == &other)
		return;

	if (value > MaxRelation)
		value = MaxRelation;
	DefaultRelations.insert_or_assign(other.ID, value);
}

void FactionInfo::SetMutualRelation(FactionInfo & other, unsigned char value)
{
	if (this == &other)
		return;

	if (value > MaxRelation)
		value = MaxRelation;
	SetRelation(other, value);
	other.SetRelation(*this, value);
}
