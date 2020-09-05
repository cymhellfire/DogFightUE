// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MenuModePawn.generated.h"

UCLASS()
class DOGFIGHT_API AMenuModePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMenuModePawn();

public:	
	// Override this function to prevent player input binding
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
