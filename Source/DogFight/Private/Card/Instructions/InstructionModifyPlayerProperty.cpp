// Dog Fight Game Code By CYM.


#include "Card/Instructions/InstructionModifyPlayerProperty.h"

#include "Game/StandardPlayerState.h"
#include "Pawns/StandardModePlayerCharacter.h"

UInstructionModifyPlayerProperty::UInstructionModifyPlayerProperty(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CameraFocusType = EInstructionCameraFocusType::ICFT_Target;
}

bool UInstructionModifyPlayerProperty::HandleActorTarget(AActor* Target)
{
	const bool Result = Super::HandleActorTarget(Target);

	if (Result && IsValid(Target))
	{
		if (APawn* Pawn = Cast<APawn>(Target))
		{
			if (AStandardPlayerState* StandardPlayerState = Pawn->GetPlayerState<AStandardPlayerState>())
			{
				switch (PropertyName)
				{
				case EModifiablePropertyName::EMPN_CardGainPerRound:
					ModifyCardGainPerRound(StandardPlayerState);
					break;
				case EModifiablePropertyName::EMPN_CardUseCount:
					ModifyCardUseCount(StandardPlayerState);
					break;
				case EModifiablePropertyName::EMPN_MaxCardCount:
					ModifyMaxCardCount(StandardPlayerState);
					break;
				default: ;
				}
			}

			if (AStandardModePlayerCharacter* StandardModePlayerCharacter = Cast<AStandardModePlayerCharacter>(Pawn))
			{
				FFormatOrderedArguments FormatArgumentValues;
				FormatArgumentValues.Add(GetOperatorText());
				FormatArgumentValues.Add(FText::FromString(NewValue));
				StandardModePlayerCharacter->MulticastAddFloatingText(FloatingText.FormatLocalizeText(FormatArgumentValues));
			}
		}
	}

	return Result;
}

void UInstructionModifyPlayerProperty::ModifyCardGainPerRound(AStandardPlayerState* PlayerState)
{
	int32 CardGainCount = PlayerState->GetCardGainNumByRound();
	CardGainCount = CalculateResult(CardGainCount);
	PlayerState->SetCardGainNumByRound(CardGainCount);
}

void UInstructionModifyPlayerProperty::ModifyCardUseCount(AStandardPlayerState* PlayerState)
{
	int32 CardUseCount = PlayerState->GetCardUseCountPerRound();
	CardUseCount = CalculateResult(CardUseCount);
	PlayerState->SetCardUseCountPerRound(CardUseCount);
}

void UInstructionModifyPlayerProperty::ModifyMaxCardCount(AStandardPlayerState* PlayerState)
{
	int32 MaxCardCount = PlayerState->GetMaxCardNum();
	MaxCardCount = CalculateResult(MaxCardCount);
	PlayerState->SetMaxCardNum(MaxCardCount);
}

int32 UInstructionModifyPlayerProperty::CalculateResult(int32 OldValue)
{
	const int32 DeltaValue = FCString::Atoi(*NewValue);
	switch(ModifyMethod)
	{
	case EPropertyModifyMethod::EPMM_Addition:
		return OldValue + DeltaValue;
	case EPropertyModifyMethod::EPMM_Subtract:
		return OldValue - DeltaValue;
	case EPropertyModifyMethod::EPMM_Replace:
		return DeltaValue;
	}

	return OldValue;
}


FText UInstructionModifyPlayerProperty::GetOperatorText()
{
	switch(ModifyMethod)
	{
	case EPropertyModifyMethod::EPMM_Addition:
		return FText::FromStringTable(ST_BUFF_LOC, TEXT("Operator_Addition"));
	case EPropertyModifyMethod::EPMM_Subtract:
		return FText::FromStringTable(ST_BUFF_LOC, TEXT("Operator_Subtract"));
	case EPropertyModifyMethod::EPMM_Replace: 
		return FText::FromStringTable(ST_BUFF_LOC, TEXT("Operator_Replace"));
	}

	return FText::FromStringTable(ST_BUFF_LOC, TEXT("Operator_Replace"));
}
