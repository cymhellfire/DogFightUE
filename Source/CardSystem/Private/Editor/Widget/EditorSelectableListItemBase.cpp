#include "Editor/Widget/EditorSelectableListItemBase.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Editor/Widget/ContextMenuWidgetBase.h"

bool UEditorSelectableListItemBase::Initialize()
{
	if (!Super::Initialize())
		return false;

	if (ItemBorder)
	{
		ItemBorder->SetBrush(DefaultBrush);
		ItemBorder->OnMouseButtonDownEvent.BindDynamic(this, &UEditorSelectableListItemBase::OnMouseButtonDownEvent);
	}

	// Create context menu widget
	// if (IsValid(ContextMenuClass) && ContextMenuContainer)
	// {
	// 	ContextMenuWidget = CreateWidget<UUserWidget>(this, ContextMenuClass);
	// 	if (ContextMenuWidget)
	// 	{
	// 		ContextMenuContainer->AddChild(ContextMenuWidget);
	// 	}
	//
	// 	ContextMenuContainer->SetVisibility(ESlateVisibility::Collapsed);
	// }

	return true;
}

void UEditorSelectableListItemBase::BeginDestroy()
{
	Super::BeginDestroy();

	if (ItemBorder)
	{
		ItemBorder->OnMouseButtonDownEvent.Clear();
	}
}

void UEditorSelectableListItemBase::NativeOnItemSelectionChanged(bool bIsSelected)
{
	if (ItemBorder)
	{
		ItemBorder->SetBrush(bIsSelected ? SelectedBrush : DefaultBrush);
	}
}

void UEditorSelectableListItemBase::OpenContextMenu(FVector2D Position)
{
	if (!bContextMenuOpened)
	{
		if (ContextMenuWidget == nullptr)
		{
			ContextMenuWidget = CreateWidget<UContextMenuWidgetBase>(this, ContextMenuClass);
			ContextMenuWidget->AddToViewport();
			ContextMenuWidget->SetPositionInViewport(Position, false);
			ContextMenuWidget->SetFocus();
			ContextMenuFocusLostHandle = ContextMenuWidget->OnContextMenuFocusLost.AddUObject(this, &UEditorSelectableListItemBase::OnContextMenuFocusLost);
			OnContextMenuOpened();
		}
		bContextMenuOpened = true;
	}
}

void UEditorSelectableListItemBase::CloseContextMenu()
{
	if (bContextMenuOpened)
	{
		if (ContextMenuWidget)
		{
			ContextMenuWidget->OnContextMenuFocusLost.Remove(ContextMenuFocusLostHandle);
			ContextMenuWidget->RemoveFromViewport();
			ContextMenuWidget->ConditionalBeginDestroy();
			ContextMenuWidget = nullptr;
		}
		bContextMenuOpened = false;
	}
}

void UEditorSelectableListItemBase::OnContextMenuFocusLost()
{
	// Close the menu when lost focus
	CloseContextMenu();
}

FEventReply UEditorSelectableListItemBase::OnMouseButtonDownEvent(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	// if (ContextMenuContainer == nullptr)
	// {
	// 	return FEventReply(false);
	// }

	if (MouseEvent.IsMouseButtonDown(FKey("RightMouseButton")))
	{
		//FVector2D RenderPos = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
		FVector2D PixelPos, ViewportPos;
		USlateBlueprintLibrary::AbsoluteToViewport(this, MouseEvent.GetScreenSpacePosition(), PixelPos, ViewportPos);

		OpenContextMenu(ViewportPos);

		// if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(ContextMenuContainer->Slot))
		// {
		// 	
		// 	CanvasPanelSlot->SetDesiredPosition(RenderPos);
		// 	CanvasPanelSlot->RebaseLayout();
		// }
		//
		// if (!bContextMenuOpened)
		// {
		// 	ContextMenuContainer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		// 	bContextMenuOpened = true;
		// 	OnContextMenuOpened();
		// }

		return FEventReply(true);
	}

	return FEventReply(false);
}
