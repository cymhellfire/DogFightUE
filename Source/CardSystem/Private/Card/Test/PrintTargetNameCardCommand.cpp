#include "Card/Test/PrintTargetNameCardCommand.h"

#include "CardSystem.h"
#include "Card/Card.h"

bool UPrintTargetNameCardCommand::Initialize(void* Param)
{
	if (!Super::Initialize(Param))
		return false;

	FPrintTargetNameCardCommandParam* InitParam = reinterpret_cast<FPrintTargetNameCardCommandParam*>(Param);
	if (InitParam == nullptr)
	{
		return false;
	}

	TargetBatch = InitParam->TargetBatch;
	return true;
}

void UPrintTargetNameCardCommand::Run()
{
	if (!OwnerCard.IsValid())
		return;

	TArray<AActor*> TargetList = OwnerCard->GetActorTargetListByBatch(TargetBatch);
	for (AActor* Target : TargetList)
	{
		UE_LOG(LogCardSystem, Log, TEXT("[PrintTargetName] Target name is %s"), *Target->GetName());
	}
}
