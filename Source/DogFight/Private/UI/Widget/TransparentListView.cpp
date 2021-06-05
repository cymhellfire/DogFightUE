// Dog Fight Game Code By CYM.


#include "UI/Widget/TransparentListView.h"

TSharedRef<STableViewBase> UTransparentListView::RebuildListWidget()
{
	TSharedRef<SListView<UObject*>> NewWidget = ConstructListView<SListView>();

	FSlateBrush TransparentBrush;
	TransparentBrush.TintColor = FSlateColor(FLinearColor{1.f, 1.f, 1.f, 0.f});

	TransparentTableViewStyle = FAppStyle::Get().GetWidgetStyle<FTableViewStyle>("ListView");
	TransparentTableViewStyle.SetBackgroundBrush(TransparentBrush);
	NewWidget->SetStyle(&TransparentTableViewStyle);

	return NewWidget;
}
