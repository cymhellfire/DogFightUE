#include "GameService/DelegateHelperService.h"

#include "Object/DelegateHelper.h"

UDelegateHelper* UDelegateHelperService::GetDelegateHelper()
{
	UDelegateHelper* Result = nullptr;
	// if (FreeHelpers.Num() == 0)
	// {
		Result = NewObject<UDelegateHelper>(this);
	// }
	// else
	// {
	// 	Result = FreeHelpers.Pop();
	// }

	// Record result to using array
	UsingHelper.AddUnique(Result);

	return Result;
}

void UDelegateHelperService::ReleaseDelegateHelper(UDelegateHelper* InHelper)
{
	if (!IsValid(InHelper))
	{
		return;
	}

	// Reset helper before reclaim
	InHelper->Reset();

	// Update record
	UsingHelper.Remove(InHelper);
	//FreeHelpers.AddUnique(InHelper);
	InHelper->ConditionalBeginDestroy();
}
