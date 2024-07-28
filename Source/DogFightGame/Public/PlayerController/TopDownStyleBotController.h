// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Controller/BotPlayerController.h"
#include "TopDownStyleBotController.generated.h"

class ATopDownStylePlayerCharacter;

UCLASS()
class DOGFIGHTGAME_API ATopDownStyleBotController : public ABotPlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATopDownStyleBotController();

	bool SpawnCharacterPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerController")
	TSubclassOf<ATopDownStylePlayerCharacter> CharacterClass;

protected:
	UPROPERTY(Transient)
	ATopDownStylePlayerCharacter* CharacterPawn;
};
