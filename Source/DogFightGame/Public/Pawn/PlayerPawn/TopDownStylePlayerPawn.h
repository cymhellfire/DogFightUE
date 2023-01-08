#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "TopDownStylePlayerPawn.generated.h"

class UTopDownStyleCameraComponent;

UCLASS()
class DOGFIGHTGAME_API ATopDownStylePlayerPawn : public ASpectatorPawn
{
	GENERATED_BODY()
public:
	ATopDownStylePlayerPawn(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UTopDownStyleCameraComponent* GetCameraComponent() const
	{
		return CameraComponent;
	}

	UFUNCTION(Server, Reliable)
	void ServerMoveCharacter(const FVector& Destination);

protected:
	UFUNCTION(BlueprintCallable, Category="TopDownStylePlayerPawn")
	void OnSetDestination();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerPawn")
	UTopDownStyleCameraComponent* CameraComponent;
};
