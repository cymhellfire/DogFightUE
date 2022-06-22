#pragma once
#include "Card/CardCommand.h"

#include "PrintTargetNameCardCommand.generated.h"

USTRUCT(BlueprintType)
struct FPrintTargetNameCardCommandParam
{
	GENERATED_BODY()

	UPROPERTY()
	int32 TargetBatch;
};

UCLASS()
class CARDSYSTEM_API UPrintTargetNameCardCommand : public UCardCommand
{
	GENERATED_BODY()
public:
	virtual bool Initialize(void* Param) override;

	virtual void Run() override;

protected:
	int32 TargetBatch;
};
