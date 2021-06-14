// Dog Fight Game Code By CYM.
#pragma once

#include "CoreMinimal.h"

#if WITH_IMGUI
struct FDebugPlayerBaseInfo
{
	bool bActive;
	int32 PlayerId;
	FString PlayerName;
	int32 HoldCards;
	int32 MaxHoldCards;
	int32 UsedCards;
	int32 MaxUseCards;
	int32 CurrentHealth;
	int32 MaxHealth;
	int32 CurrentStrength;
	int32 MaxStrength;

	FDebugPlayerBaseInfo()
	{
		// Fill with game default value
		bActive = false;
		PlayerId = -1;
		PlayerName = TEXT("");
		HoldCards = 0;
		MaxHoldCards = 2;
		UsedCards = 0;
		MaxUseCards = 2;
		MaxHealth = 100;
		CurrentHealth = MaxHealth;
		MaxStrength = 50;
		CurrentStrength = MaxStrength;
	}
};

struct FDebugPlayerRelationInfo
{
	int32 PlayerId;
	FString PlayerName;
	int32 RelationPoint;
	int32 ReceiveDamage;
	int32 CurrentHealth;

	FDebugPlayerRelationInfo() :
		PlayerId(-1), PlayerName(TEXT("")),
		RelationPoint(0), ReceiveDamage(0),
		CurrentHealth(0)
	{
	}

	FDebugPlayerRelationInfo(struct FPlayerRelationStatistic PlayerRelationStatistic);
};

struct FDebugGamePhaseHistoryRecord
{
	FName GamePhaseName;
	int32 PlayerId;
	int32 TimeMinutes;
	int32 TimeSeconds;
	uint8 SwitchMethod;

	TArray<FString> ExtraEvents;

	void AddGiveCardEvent(FString CardName)
	{
		ExtraEvents.Add(FString::Printf(TEXT("Gain card: %s"), *CardName));
	}

	void AddUseCardEvent(FString CardName)
	{
		ExtraEvents.Add(FString::Printf(TEXT("Use card: %s"), *CardName));
	}

	void AddDiscardCardEvent(FString CardName)
	{
		ExtraEvents.Add(FString::Printf(TEXT("Discard card: %s"), *CardName));
	}
};
#endif