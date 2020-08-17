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

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(Category=PlayerController, EditAnywhere, BlueprintReadOnly, Replicated)
	TSubclassOf<AStandardModePlayerCharacter> CharacterPawnClass;

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void ProcessPlayerInput(const float DeltaTime, const bool bGamePaused) override;

	/* Navigate character to current mouse cursor position. */
	void MoveToMouseCursor() const;

	void GetMouseCursorPosition();

	UFUNCTION(Server, Unreliable)
	void CmdUploadMouseCursorPosition(FVector Position);

	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	UFUNCTION(Server, Reliable)
	void CmdStartMovement();
	UFUNCTION(Server, Reliable)
	void CmdStopMovement();

	UFUNCTION(Server, Reliable)
	void SpawnCharacterPawn();
private:
	UPROPERTY(Category=PlayerController, VisibleAnywhere, Replicated)
	AStandardModePlayerCharacter* CharacterPawn;

	UPROPERTY(Category=PlayerController, VisibleAnywhere, Replicated)
	bool bMoveToMouseCursor;

	UPROPERTY(Category=PlayerController, VisibleAnywhere)
	FVector CursorWorldPosition;
};
