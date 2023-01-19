#include "Card/CardDescObject.h"
#include "CardSystem.h"
#include "Net/UnrealNetwork.h"

void UCardDescObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(UCardDescObject, CardName, SharedParams);
}

void UCardDescObject::SetCardName(FString InName)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UCardDescObject, CardName, this);
	CardName = InName;
}
