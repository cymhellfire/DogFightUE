// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InGameUIInteractComponent.generated.h"

class UEnhancedInputComponent;
class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOGFIGHTGAME_API UInGameUIInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInGameUIInteractComponent();

	void InitializeInput(UEnhancedInputComponent* InputComponent);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSelectedCharacter();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UIInteractComponent")
	UInputAction* SelectCharacterAction;
};
