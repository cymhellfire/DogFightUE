// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "StandardPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AStandardPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerNameChangedSignature, const FString&, NewName);

	FPlayerNameChangedSignature OnPlayerNameChanged;

	virtual void OnRep_PlayerName() override;
};
