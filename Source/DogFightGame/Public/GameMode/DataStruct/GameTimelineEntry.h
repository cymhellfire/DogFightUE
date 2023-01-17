#pragma once

class FGameTimelineEntry
{
public:
	FGameTimelineEntry(int32 InPlayerId);

	void SetPriority(int32 InValue);

	int32 GetPlayerId() const
	{
		return PlayerId;
	}

	int32 GetPriority() const
	{
		return Priority;
	}

protected:

	int32 PlayerId;

	int32 Priority;
};
