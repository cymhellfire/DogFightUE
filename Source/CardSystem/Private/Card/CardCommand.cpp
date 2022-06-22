#include "Card/CardCommand.h"
#include "Card/Card.h"

bool UCardCommand::Initialize(void* Param)
{
	OwnerCard = GetTypedOuter<UCard>();
	return OwnerCard.IsValid();
}
