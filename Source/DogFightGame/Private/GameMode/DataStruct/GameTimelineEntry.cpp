#include "GameMode/DataStruct/GameTimelineEntry.h"
#include "Common/CommonMagicNumber.h"

FGameTimelineEntry::FGameTimelineEntry(int32 InId, IGameTimelineEntityInterface* InEntity)
	: Id(InId)
	, Priority(0)
	, EntityType(EGameTimelineEntityType::None)
	, OwnerEntity(InEntity)
{
	InEntity->EntityId = InId;
}

void FGameTimelineEntry::SetPriority(int32 InValue)
{
	Priority = InValue;
}

void FGameTimelineEntry::SetType(EGameTimelineEntityType::Type InType)
{
	EntityType = InType;

	DebugString = GetDebugString();
}

int32 FGameTimelineEntry::GetOwnerPlayerId() const
{
	if (OwnerEntity)
	{
		return OwnerEntity->GetOwnerPlayerId();
	}

	return GameFlowMagicNumbers::InvalidPlayerId;
}

FGameTimelineEntryDescObject FGameTimelineEntry::GetDescObject() const
{
	FGameTimelineEntryDescObject NewDescObject{
		GetOwnerPlayerId(),
		GetDebugString(),
		Cast<AActor>(OwnerEntity)
	};
	return NewDescObject;
}

FString FGameTimelineEntry::GetDebugString() const
{
	switch(EntityType)
	{
	case EGameTimelineEntityType::Player:
		return FString::Printf(TEXT("[%d] Player-%d"), Id, GetOwnerPlayerId());
	case EGameTimelineEntityType::Bot:
		return FString::Printf(TEXT("[%d] Bot-%d"), Id, GetOwnerPlayerId());
	case EGameTimelineEntityType::None:
	default:
		break;
	}

	return "Invalid Entity";
}
