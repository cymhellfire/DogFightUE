// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/AvatarDataAsset.h"
#include "GameFramework/Character.h"
#include "ActionGameCharacter.generated.h"

struct FStreamableHandle;

UCLASS()
class ACTIONGAME_API AActionGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionGameCharacter();

	UFUNCTION(BlueprintCallable, Category="ActionGameCharacter")
	void SetupAvatarAppearance(const FAvatarDescData& AvatarDescData);

	UFUNCTION(BlueprintCallable, Category="ActionGameCharacter")
	void SetupAvatarAppearanceWithAsset(UAvatarDataAsset* InAsset);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ApplyAvatarDesc(const FAvatarDescData& AvatarDescData);

private:
	FGuid AsyncLoadGuid;
};
