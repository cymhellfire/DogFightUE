#include "GameService/CardEditorService.h"

#include "CardSystem.h"
#include "Editor/CardEditor.h"
#include "Editor/Widget/CardEditorWidget.h"
#include "Editor/Widget/CommandListPopupWidget.h"
#include "Editor/Widget/EditorTextInputPopupWidget.h"
#include "GameService/CommandManagerService.h"

UCardEditorService* UCardEditorService::GetCardEditorService()
{
	return UGameService::GetGameServiceBySuperClass<UCardEditorService>();
}

void UCardEditorService::OpenCardEditor()
{
	if (!IsValid(EditorWidgetClass))
	{
		return;
	}

	if (CurrentCardEditor == nullptr)
	{
		APlayerController* OwnerPC = nullptr;
		if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
		{
			OwnerPC = GameInstance->GetFirstLocalPlayerController();
		}

		if (!OwnerPC)
		{
			UE_LOG(LogCardSystem, Error, TEXT("[CardEditorService] Failed to get player controller."));
			return;
		}

		CurrentCardEditor = Cast<UCardEditorWidget>(CreateWidget(OwnerPC, EditorWidgetClass, "CardEditor"));
	}

	if (CurrentCardEditor)
	{
		CurrentCardEditor->AddToViewport();
		InitializeCardEditor();
		CurrentCardEditor->BindCardEditor(CardEditor);
	}
}

void UCardEditorService::CloseCardEditor()
{
	if (CurrentCardEditor)
	{
		CurrentCardEditor->RemoveFromViewport();
	}
}

void UCardEditorService::OpenCommandListPopupWidget()
{
	if (!IsValid(CommandListPopupWidgetClass))
	{
		return;
	}

	APlayerController* OwnerPC = nullptr;
	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		OwnerPC = GameInstance->GetFirstLocalPlayerController();
	}

	if (!OwnerPC)
	{
		UE_LOG(LogCardSystem, Error, TEXT("[CardEditorService] Failed to get player controller."));
		return;
	}

	UCommandListPopupWidget* Popup = Cast<UCommandListPopupWidget>(CreateWidget(OwnerPC, CommandListPopupWidgetClass, "CommandListPopup"));
	// Initialize
	if (UCommandManagerService* CommandManagerService = GetGameServiceBySuperClass<UCommandManagerService>())
	{
		Popup->SetClassList(CommandManagerService->GetAllCommandVisualItems());
	}
	Popup->AddToViewport();
}

UEditorTextInputPopupWidget* UCardEditorService::OpenTextInputPopupWidget(const FTextInputPopupParameter& InParam)
{
	if (!IsValid(EditorTextInputPopupWidgetClass))
	{
		return nullptr;
	}

	APlayerController* OwnerPC = nullptr;
	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		OwnerPC = GameInstance->GetFirstLocalPlayerController();
	}

	if (!OwnerPC)
	{
		UE_LOG(LogCardSystem, Error, TEXT("[CardEditorService] Failed to get player controller."));
		return nullptr;
	}

	UEditorTextInputPopupWidget* Popup = Cast<UEditorTextInputPopupWidget>(CreateWidget(OwnerPC, EditorTextInputPopupWidgetClass, "EditorTextInputPopup"));
	Popup->ApplyParam(InParam);
	Popup->AddToViewport();

	return Popup;
}

void UCardEditorService::InitializeCardEditor()
{
	if (CardEditor.IsValid())
	{
		return;
	}

	CardEditor = MakeShareable(new FCardEditor);
}
