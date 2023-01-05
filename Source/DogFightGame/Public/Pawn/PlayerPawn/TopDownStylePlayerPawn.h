#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "TopDownStylePlayerPawn.generated.h"

class UTopDownStyleCameraComponent;

UCLASS()
class DOGFIGHTGAME_API ATopDownStylePlayerPawn : public ADefaultPawn
{
	GENERATED_BODY()
public:
	ATopDownStylePlayerPawn(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UTopDownStyleCameraComponent* GetCameraComponent() const
	{
		return CameraComponent;
	}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerPawn")
	UTopDownStyleCameraComponent* CameraComponent;
};
