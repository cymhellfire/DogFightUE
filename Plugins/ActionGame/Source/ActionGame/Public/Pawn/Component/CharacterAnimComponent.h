// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Common/ActionAnimCommon.h"
#include "Components/ActorComponent.h"
#include "CharacterAnimComponent.generated.h"

struct FAvatarAnimSetData;
class UMotionWarpingComponent;

USTRUCT(BlueprintType)
struct FCharacterAnimWarpingParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="CharacterAnimWarpingParams")
	FName TargetName;

	UPROPERTY(BlueprintReadWrite, Category="CharacterAnimWarpingParams")
	FVector TargetPos;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONGAME_API UCharacterAnimComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterAnimComponent();

	void SetupPredefineAnimations(const FAvatarAnimSetData& InData);

	void RefreshAnimInstance();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayMontage(UAnimMontage* InMontage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayMontageWithWarping(UAnimMontage* InMontage, const FCharacterAnimWarpingParams& WarpingParams);

	FCharacterAnimWarpingParams* GetAnimWarpingParams(UAnimMontage* InMontage)
	{
		return AnimWarpingDataMap.Find(InMontage);
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* InMontage, bool bInterrupted);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


	virtual float PlayAnimation(UAnimMontage* InAnimMontage);

	virtual float PlayAnimationWithWarping(UAnimMontage* InAnimMontage, FName TargetName, const FVector& TargetPos);

	virtual float PlayPredefineAnimation(EActionAnimPredefinedType::Type InType);

	virtual float PlayPredefineAnimationWithWarping(EActionAnimPredefinedType::Type InType, FName TargetName, const FVector& TargetPos);

private:
	TWeakObjectPtr<UAnimInstance> AnimInstance;
	TWeakObjectPtr<UAnimMontage> WaitingMontage;
	TWeakObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	TMap<TWeakObjectPtr<UAnimMontage>, FCharacterAnimWarpingParams> AnimWarpingDataMap;

	UPROPERTY(Transient)
	TMap<TEnumAsByte<EActionAnimPredefinedType::Type>, UAnimationAsset*> PredefineAnimMap;
};
