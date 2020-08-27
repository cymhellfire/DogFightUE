// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "StandardModePlayerController.generated.h"

class AStandardModePlayerCharacter;

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AStandardModePlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

	void SetClickMovementEnabled(bool bEnabled);

	UPROPERTY(Category=PlayerController, EditAnywhere, BlueprintReadOnly, Replicated)
	TSubclassOf<AStandardModePlayerCharacter> CharacterPawnClass;

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void ProcessPlayerInput(const float DeltaTime, const bool bGamePaused) override;

	void OnSetDestinationPressed();

	UFUNCTION(Server, Reliable)
	void CmdMoveToMouseCursor(FVector Destination);

	UFUNCTION(Server, Reliable)
	void SpawnCharacterPawn();
private:
	UPROPERTY(Category=PlayerController, VisibleAnywhere, Replicated)
	AStandardModePlayerCharacter* CharacterPawn;

	/** Is click movement enabled for this player controller? */
	UPROPERTY(Category=PlayerController, VisibleAnywhere, Replicated)
	bool bClickMoveEnabled;
};
