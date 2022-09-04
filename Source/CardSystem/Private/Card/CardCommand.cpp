#include "Card/CardCommand.h"
#include "Card/Card.h"

bool UCardCommand::Initialize(void* Param)
{
	OwnerCard = GetTypedOuter<UCard>();
	return OwnerCard.IsValid();
}

FString UCardCommand::GetNameString() const
{
	return GetClass()->GetName();
}

FString UCardCommand::GetDescString() const
{
	return FString::Printf(TEXT("%s_Desc"), *GetClass()->GetName());
}

ECardCommandType UCardCommand::GetCommandType() const
{
	return CommandType;
}
