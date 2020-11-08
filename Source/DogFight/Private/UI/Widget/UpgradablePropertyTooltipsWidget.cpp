// Dog Fight Game Code By CYM.


#include "UpgradablePropertyTooltipsWidget.h"

FText UUpgradablePropertyTooltipsWidget::GetLocalizedText(const FUpgradablePropertyDisplayInfo& DisplayInfo) const
{
	return DisplayInfo.GetLocalizedText();
}

FText UUpgradablePropertyTooltipsWidget::GetLocalizedArgument(const FUpgradablePropertyDisplayInfo& DisplayInfo, int32 Index) const
{
	return DisplayInfo.GetLocalizedArgument(Index);
}
