// Dog Fight Game Code By CYM.

#pragma once

#include "CoreMinimal.h"
#include "Components/ListView.h"
#include "TransparentListView.generated.h"

/**
 * A List View class with transparent background.
 */
UCLASS()
class DOGFIGHT_API UTransparentListView : public UListView
{
	GENERATED_BODY()

protected:
	virtual TSharedRef<STableViewBase> RebuildListWidget() override;

protected:
	FTableViewStyle TransparentTableViewStyle;
};
