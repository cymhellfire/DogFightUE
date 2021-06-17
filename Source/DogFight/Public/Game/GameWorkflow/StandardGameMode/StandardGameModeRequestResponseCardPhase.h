// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "StandardGameModePhaseBase.h"
#include "StandardGameModeRequestResponseCardPhase.generated.h"

class ACardBase;
class AStandardPlayerState;

/**
 * Game phase that request a player to use response card.
 */
UCLASS()
class UStandardGameModeRequestResponseCardPhase : public UStandardGameModePhaseBase
{
	GENERATED_BODY()
public:
	UStandardGameModeRequestResponseCardPhase(const FObjectInitializer& ObjectInitializer);

	void InitializeResponseCardPhase(int32 PlayerId, TArray<TSubclassOf<ACardBase>> ResponseAllowCards, AActor* SourceActor, const FText& CardToResponse);

protected:
	virtual bool StartPhase() override;

	UFUNCTION()
	void OnResponseCardSelected(ACardBase* SelectedCard, AStandardPlayerState* ResponsePlayerState);

protected:
	int32 TargetPlayerId;
	TArray<TSubclassOf<ACardBase>> ResponseCardClasses;
	AActor* RequestSource;
	FText CardNameToResponse;
};
