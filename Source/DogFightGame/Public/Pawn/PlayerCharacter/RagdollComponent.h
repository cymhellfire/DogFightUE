#pragma once

#include "RagdollComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRagdollComponentActiveStatusChangedEvent, bool, bActive);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRagdollComponentOnCachePoseEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRagdollComponentPostCachePoseEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRagdollComponentTotallyRecoveredEvent);

UCLASS(BlueprintType)
class DOGFIGHTGAME_API URagdollComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URagdollComponent();

	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void ServerSetRagdollActive(bool bActive);

	bool IsRagdollActive() const
	{
		return bRagdoll;
	}

private:
	UFUNCTION(NetMulticast, Reliable)
	void SetRagdollActive(bool bActive);

	void PreCacheRagdollPose();

	void SynchronizeOrientationWithRagdoll(bool bIsFacingUp);

	void DoCacheRagdollPose();

	void PostCacheRagdollPose();

	void OnRecoveredFromRagdoll();

	UFUNCTION()
	void OnRep_SyncRagdollRotation();

public:
	UPROPERTY(BlueprintAssignable, Category="RagdollComponent")
	FRagdollComponentActiveStatusChangedEvent OnRagdollStatusChanged;

	UPROPERTY(BlueprintAssignable, Category="RagdollComponent")
	FRagdollComponentOnCachePoseEvent OnRagdollCachedPose;

	UPROPERTY(BlueprintAssignable, Category="RagdollComponent")
	FRagdollComponentPostCachePoseEvent PostRagdollCachedPose;

	UPROPERTY(BlueprintAssignable, Category="RagdollComponent")
	FRagdollComponentTotallyRecoveredEvent OnRagdollRecovered;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="RagdollComponent")
	FName NeckBoneName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="RagdollComponent")
	FName PelvisBoneName;

	/** Name of bone that provide synchronize position for update component. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="RagdollComponent")
	FName FollowBoneName;

	/** Use to detect ground below current position. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="RagdollComponent")
	float GroundDetectHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="RagdollComponent")
	FName PoseSlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="RagdollComponent")
	UAnimMontage* GetUpFromFaceMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="RagdollComponent")
	UAnimMontage* GetUpFromBackMontage;

private:
	uint8 bRagdoll:1;
	uint8 bRagdollFacingUp:1;

	/** The initial local position of SkeletalMesh component. */
	FVector MeshComponentOffset;

	FTimerHandle RagdollRecoverTimerHandle;

	UPROPERTY(Transient)
	ACharacter* OwnerCharacter;

	/** Indicate the component that moved by ragdoll. */
	UPROPERTY(Transient)
	USceneComponent* UpdateComponent;

	UPROPERTY(ReplicatedUsing=OnRep_SyncRagdollRotation)
	FQuat SyncRagdollRotation;
};
