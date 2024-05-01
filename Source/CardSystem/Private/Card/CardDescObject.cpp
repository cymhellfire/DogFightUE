#include "Card/CardDescObject.h"
#include "CardSystem.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

void UCardDescObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(UCardDescObject, CardName, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UCardDescObject, CardDesc, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UCardDescObject, CardPicturePath, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UCardDescObject, CardInstanceId, SharedParams);
}

void UCardDescObject::SetCardName(FText InName)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UCardDescObject, CardName, this);
	CardName = InName;
}

void UCardDescObject::SetCardDesc(const FText& InDesc)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UCardDescObject, CardDesc, this);
	CardDesc = InDesc;
}

void UCardDescObject::SetCardPicturePath(FString InPath)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UCardDescObject, CardPicturePath, this);
	CardPicturePath = InPath;
}

void UCardDescObject::SetCardInstanceId(int32 InId)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(UCardDescObject, CardInstanceId, this);
	CardInstanceId = InId;
}

void UCardDescObject::OnRep_CardDesc()
{
	OnDescUpdated.Broadcast();
}
