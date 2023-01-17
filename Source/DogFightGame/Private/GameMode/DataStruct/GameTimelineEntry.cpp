#include "GameMode/DataStruct/GameTimelineEntry.h"

FGameTimelineEntry::FGameTimelineEntry(int32 InPlayerId)
	: PlayerId(InPlayerId)
	, Priority(0)
{
}

void FGameTimelineEntry::SetPriority(int32 InValue)
{
	Priority = InValue;
}
