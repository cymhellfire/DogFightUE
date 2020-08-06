// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "StandardModePlayerPawn.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AStandardModePlayerPawn : public ASpectatorPawn
{
	GENERATED_UCLASS_BODY()

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(Category=CameraActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStandardModeCameraComponent* StandardModeCameraComponent;

public:
	/* Return a pointer to the StandardModeCameraComponent attached to this pawn. */
	UStandardModeCameraComponent* GetStandardModeCameraComponent() const;
};
