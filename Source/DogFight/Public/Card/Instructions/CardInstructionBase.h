// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "Card/GameCardTypes.h"
#include "UObject/ObjectMacros.h"

#include "CardInstructionBase.generated.h"

class ACardBase;

USTRUCT(BlueprintType)
struct FInstructionMetaBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Instruction")
	FString InstructionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Instruction")
	ECardInstructionExecuteType ExecuteType;

	FInstructionMetaBase()
	{
		InstructionName = TEXT("NewInstruction");
		ExecuteType = ECardInstructionExecuteType::Synchronous;
	}
};

UENUM(BlueprintType)
enum class EInstructionCameraFocusType : uint8
{
	ICFT_None		UMETA(DisplayName="None", Tooltip="No camera focus event."),
	ICFT_User		UMETA(DisplayName="User", Tooltip="Focus on card user."),
	ICFT_Target		UMETA(DisplayName="Target", Tooltip="Focus on instruction target."),
};

/**
 * Base class of all kinds of Instruction.
 */
UCLASS(Blueprintable, Abstract)
class DOGFIGHT_API UCardInstructionBase : public UObject
{
	GENERATED_BODY()

public:
	UCardInstructionBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Instruction")
	FORCEINLINE ACardBase* GetOwnerCard() const { return OwnerCard; }

	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Instruction")
	void SetOwnerCard(ACardBase* Card)
	{
		OwnerCard = Card;
	}

	virtual bool Tick(float DeltaSeconds);

	virtual void ApplyMeta(const FInstructionMetaBase* MetaData);

	/** Let the Execute function run in next frame. */
	virtual void BeginExecute();

	/** Function holds all operation to do. */
	virtual void Execute();

	/** Name of this instruction. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Instruction")
	FString InstructionName;

	/** Execution type. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Instruction")
	ECardInstructionExecuteType ExecuteType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	EInstructionCameraFocusType CameraFocusType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Instruction")
	bool bForciblyCameraEventToOwner;

	UFUNCTION(BlueprintCallable, Category="CustomizableCard|Instruction")
	virtual void Finish();

protected:

	UFUNCTION(BlueprintImplementableEvent, Category="CustomizableCard|Instruction", meta=(DisplayName="Begin Execute"))
	void BeginExecuteBP();

	APawn* GetOwnerControlledPawn() const;

	bool bIsFinished;

	ACardBase* OwnerCard;

	FTickerDelegate TickDelegate;
	FDelegateHandle TickDelegateHandle;
};
