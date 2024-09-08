#pragma once

#include "CoreMinimal.h"
#include "Interface/GameTimelineEntityInterface/GameTimelineEntityInterface.h"

UENUM(BlueprintType)
namespace EGameTimelineEntityType
{
	enum Type
	{
		None,
		Player,
		Bot,
	};
}

/**
 * Entry that constructs game timeline.
 */
class FGameTimelineEntry
{
public:
	FGameTimelineEntry(int32 InId, IGameTimelineEntityInterface* InEntity);

	void SetPriority(int32 InValue);

	void SetType(EGameTimelineEntityType::Type InType);

	int32 GetId() const
	{
		return Id;
	}

	int32 GetPriority() const
	{
		return Priority;
	}

	EGameTimelineEntityType::Type GetType() const
	{
		return EntityType;
	}

	template<class T>
	T* GetTypedEntity() const
	{
		return CastChecked<T>(OwnerEntity);
	}

	int32 GetOwnerPlayerId() const;

	FString GetDebugString() const;

protected:

	int32 Id;
	int32 Priority;

	FString DebugString;

	EGameTimelineEntityType::Type EntityType;

	IGameTimelineEntityInterface* OwnerEntity;
};
