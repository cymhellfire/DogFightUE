#pragma once

#include "CoreMinimal.h"
#include "CardTargetProviderInterface.generated.h"

UENUM(BlueprintType)
enum class ECardTargetType : uint8
{
	CTT_None,
	CTT_Actor,
	CTT_Point,
	CTT_Direction,
};

USTRUCT(BlueprintType)
struct FTargetAcquireSettings
{
	GENERATED_BODY()

	FTargetAcquireSettings()
	{
		Type = ECardTargetType::CTT_None;
		TargetCount = 1;
	}

	UPROPERTY()
	ECardTargetType Type;

	UPROPERTY()
	int32 TargetCount;
};

USTRUCT(BlueprintType)
struct FAcquiredTargetInfo
{
	GENERATED_BODY()

	FAcquiredTargetInfo()
	{
		Type = ECardTargetType::CTT_None;
		ActorPtr = nullptr;
		Vector = FVector::ZeroVector;
	}

	UPROPERTY()
	ECardTargetType Type;

	UPROPERTY()
	AActor* ActorPtr;

	UPROPERTY()
	FVector Vector;
};

/**
 * PlayerController implements this interface is able to provide target to using card.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UCardTargetProviderInterface : public UInterface
{
	GENERATED_BODY()
};

class CARDSYSTEM_API ICardTargetProviderInterface
{
	GENERATED_BODY()
public:
	/**
	 * Start target acquiring process with specified settings.
	 * @param Settings			Target acquiring setting.
	 * @param Callback			Callback function when acquiring finished.
	 * @note We assumed all target returns should have same type
	 */
	virtual void StartAcquireTargets(FTargetAcquireSettings Settings, TFunction<void(bool bSuccess, TArray<FAcquiredTargetInfo>)> Callback) = 0;
};
