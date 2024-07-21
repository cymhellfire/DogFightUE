// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/AvatarDataAsset.h"
#include "GameFramework/Character.h"
#include "Interface/ActionCharacterInterface.h"
#include "ActionGameCharacter.generated.h"

class UCharacterAnimComponent;

UCLASS()
class ACTIONGAME_API AActionGameCharacter : public ACharacter, public IActionCharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionGameCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category="ActionGameCharacter")
	void SetupAvatarAppearance(const FAvatarDescData& AvatarDescData);

	UFUNCTION(BlueprintCallable, Category="ActionGameCharacter")
	void SetupAvatarAppearanceWithAsset(UAvatarDataAsset* InAsset);

	UFUNCTION(BlueprintCallable, Category="ActionGameCharacter")
	UCharacterAnimComponent* GetAnimComponent() const
	{
		return AnimComponent;
	}

#pragma region IActionCharacterInterface
	virtual float PlayActionAnimation(UAnimMontage* InMontage) override;
	virtual float PlayActionAnimationWithWarping(UAnimMontage* InMontage, FName TargetName, const FVector& TargetPos) override;
	virtual void MoveToTarget(const FVector& Target, float StopDistance) override {}
#pragma endregion IActionCharacterInterface

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ApplyAvatarDesc(const FAvatarDescData& AvatarDescData);

	void ApplyAnimationSet(const FAvatarAnimSetData& AvatarAnimSetData);

#pragma region IActionCharacterInterface
	virtual ACharacter* GetCharacter() override
	{
		return this;
	}
#pragma endregion IActionCharacterInterface

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AActionGameCharacter")
	UCharacterAnimComponent* AnimComponent;

private:
	FGuid AsyncLoadGuid;
};
