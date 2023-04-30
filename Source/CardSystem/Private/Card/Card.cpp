#include "Card/Card.h"

#include "CardSystem.h"
#include "Card/CardDescObject.h"
#include "Card/CardLogic.h"
#include "CardModifier/CardModifier.h"

UCard::UCard()
{
}

void UCard::InitDescObject()
{
	// Create description object
	DescObject = NewObject<UCardDescObject>(GetOuter(), NAME_None, RF_Transient);

	// Setup card description object use logic
	if (IsValid(CardLogic))
	{
		CardLogic->SetupCardDescObject(DescObject);
	}
}

void UCard::SetInstanceId(int32 InId)
{
	if (InId == CardInstanceId)
	{
		return;
	}

	CardInstanceId = InId;
	// Also sync to desc object
	if (DescObject)
	{
		DescObject->SetCardInstanceId(InId);
	}
}

void UCard::AddAttribute(const FAttributeCreateArgument& InArgument)
{
	if (DescObject)
	{
		DescObject->AddAttribute(InArgument);
	}
}

bool UCard::GetAttributeBoolValue(FName InName, bool& OutValue)
{
	if (DescObject)
	{
		return DescObject->GetAttributeBoolValue(InName, OutValue);
	}

	return false;
}

bool UCard::GetAttributeIntegerValue(FName InName, int32& OutValue)
{
	if (DescObject)
	{
		return DescObject->GetAttributeIntegerValue(InName, OutValue);
	}

	return false;
}

bool UCard::GetAttributeFloatValue(FName InName, float& OutValue)
{
	if (DescObject)
	{
		return DescObject->GetAttributeFloatValue(InName, OutValue);
	}

	return false;
}

void UCard::AddModifierObject(UCardModifier* InModifier)
{
	if (DescObject)
	{
		DescObject->AddModifierObject(InModifier);
	}
}

void UCard::RemoveModifierObject(UCardModifier* InModifier)
{
	if (DescObject)
	{
		DescObject->RemoveModifierObject(InModifier);
	}
}

void UCard::SetCardLogicPath(const FString& InPath)
{
	LogicScriptPath = InPath;

	// Create logic
	CreateCardLogic();

	// Update the desc object after logic set
	InitDescObject();
}

void UCard::CreateCardLogic()
{
	CardLogic = NewObject<UCardLogic>(this, TEXT("CardLogic"), RF_Transient);
	if (IsValid(CardLogic))
	{
		CardLogic->OnCardLogicFinished.AddDynamic(this, &UCard::OnCardLogicFinished);
		CardLogic->InitLogic(this, LogicScriptPath);
	}
}

void UCard::StartCardLogic()
{
	if (IsValid(CardLogic))
	{
		// Start the logic
		CardLogic->StartLogic();
	}
}

/**
 * Execution progress can be seperated into two parts:
 * ------------------------------------------------
 * |  Select Targets  |  Card Logic with Targets  |
 * ------------------------------------------------
 */
void UCard::Execute()
{
	if (!LogicScriptPath.IsEmpty())
	{
		StartCardLogic();
	}
}

void UCard::SetOwnerController(AController* InOwner)
{
	if (!IsValid(InOwner))
	{
		UE_LOG(LogCardSystem, Error, TEXT("[Card] Owner controller cannot be nullptr."));
		return;
	}

	// Check if this controller implemented specified interfaces
	if (InOwner->Implements<UCardTargetProviderInterface>())
	{
		OwnerController = InOwner;
	}
	else
	{
		UE_LOG(LogCardSystem, Error, TEXT("[Card] Owner controller must implement UCardTargetProviderInterface."));
	}
}

void UCard::SetOwnerPlayerId(int32 PlayerId)
{
	OwnerPlayerId = PlayerId;
}

void UCard::OnCardCancel()
{
	UE_LOG(LogCardSystem, Log, TEXT("[Card] %s card cancelled."), *GetName());

	// Broadcast cancel event
	OnCardExecutionFinished.Broadcast(ECardExecutionResult::CER_Cancelled, this);
}

void UCard::OnCardLogicFinished(ECardLogicFinishType::Type FinishType)
{
	if (FinishType == ECardLogicFinishType::Success)
	{
		OnCardFinished();
	}
	else
	{
		OnCardCancel();
	}
}

void UCard::OnCardFinished()
{
	UE_LOG(LogCardSystem, Log, TEXT("[Card] %s card finished."), *GetName());

	// Broadcast finish event
	OnCardExecutionFinished.Broadcast(ECardExecutionResult::CER_Default, this);
}
