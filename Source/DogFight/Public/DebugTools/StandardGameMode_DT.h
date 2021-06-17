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

	FDebugPlayerRelationInfo(struct FPlayerRelationStatistic& PlayerRelationStatistic);
};

struct FDebugGamePhaseHistoryRecord
{
	FName GamePhaseName;
	int32 PlayerId;
	int32 TimeMinutes;
	int32 TimeSeconds;
	uint8 SwitchMethod;

	TArray<FString> ExtraEvents;

	void AddGiveCardEvent(FString CardName, int32 EventPlayerId)
	{
		ExtraEvents.Add(FString::Printf(TEXT("Player[%d] gain card: %s"), EventPlayerId, *CardName));
	}

	void AddUseCardEvent(FString CardName, int32 EventPlayerId)
	{
		ExtraEvents.Add(FString::Printf(TEXT("Player[%d] use card: %s"), EventPlayerId, *CardName));
	}

	void AddDiscardCardEvent(FString CardName, int32 EventPlayerId)
	{
		ExtraEvents.Add(FString::Printf(TEXT("Player[%d] discard card: %s"), EventPlayerId, *CardName));
	}

	void AddResponseCardEvent(FString CardName, int32 EventPlayerId)
	{
		ExtraEvents.Add(FString::Printf(TEXT("Player[%d] use response card: %s"), EventPlayerId, *CardName));
	}

	void AddSelectActorEvent(AActor* Actor, int32 EventPlayerId);

	void AddSelectPointEvent(FVector Point, int32 EventPlayerId)
	{
		ExtraEvents.Add(FString::Printf(TEXT("Player[%d] targeting point: %s"), EventPlayerId, *Point.ToString()));
	}

	void AddSelectDirectionEvent(FVector Direction, int32 EventPlayerId)
	{
		ExtraEvents.Add(FString::Printf(TEXT("Player[%d] targeting direction: %s"), EventPlayerId, *Direction.ToString()));
	}
};
#endif