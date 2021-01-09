// Dog Fight Game Code By CYM.


#include "CardDescriptionItemWidget.h"

void UCardDescriptionItemWidget::ApplyItemInfo(const FCardDescriptionItemInfo& ItemInfo)
{
	OwningItemInfo = ItemInfo;

	// Invoke Blueprint implementation
	K2_OnApplyItemInfo();
}

FText UCardDescriptionItemWidget::GetLocalizedText() const
{
	return OwningItemInfo.GetLocalizedText();
}

FText UCardDescriptionItemWidget::GetLocalizedExtraArgumentText(int32 Index) const
{
	return OwningItemInfo.GetLocalizedExtraArgumentText(Index);
}

FUpgradablePropertyDisplayInfo UCardDescriptionItemWidget::GetPropertyDisplayInfo(int32 PropertyIndex) const
{
	return OwningItemInfo.GetPropertyDisplayInfo(PropertyIndex);
}
