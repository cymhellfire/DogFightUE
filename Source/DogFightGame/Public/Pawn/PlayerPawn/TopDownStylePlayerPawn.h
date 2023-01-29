#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "TopDownStylePlayerPawn.generated.h"

class UInputAction;
class UTopDownStyleCameraComponent;

UCLASS()
class DOGFIGHTGAME_API ATopDownStylePlayerPawn : public ASpectatorPawn
{
	GENERATED_BODY()
public:
	ATopDownStylePlayerPawn(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UTopDownStyleCameraComponent* GetCameraComponent() const
	{
		return CameraComponent;
	}

	UFUNCTION(Server, Reliable)
	void ServerMoveCharacter(const FVector& Destination);

	UFUNCTION(Server, Reliable)
	void ServerSetCharacterMovable(bool bEnable);

protected:
	UFUNCTION()
	void OnSetDestination(const FInputActionInstance& Instance);

	UFUNCTION()
	void OnMoveInputTriggered(const FInputActionInstance& Instance);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TopDownStylePlayerPawn")
	UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TopDownStylePlayerPawn")
	UInputAction* SetCharDestAction;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerPawn")
	UTopDownStyleCameraComponent* CameraComponent;

	UPROPERTY(Replicated)
	uint8 bEnableCharacterMove : 1;
};
