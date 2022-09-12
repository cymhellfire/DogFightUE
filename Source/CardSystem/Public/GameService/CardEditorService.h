#pragma once

#include "CoreMinimal.h"
#include "Common/CardEditorType.h"
#include "GameService/GameService.h"
#include "CardEditorService.generated.h"

class UCardEditorWidget;
class UCommandListPopupWidget;
class UEditorTextInputPopupWidget;
class FCardEditor;

UCLASS(Abstract, Blueprintable)
class CARDSYSTEM_API UCardEditorService : public UGameService
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category="CardEditor")
	static UCardEditorService* GetCardEditorService();

	UFUNCTION(BlueprintCallable, Category="CardEditor")
	void OpenCardEditor();

	UFUNCTION(BlueprintCallable, Category="CardEditor")
	void CloseCardEditor();

	UFUNCTION(BlueprintCallable, Category="CardEditor")
	UCommandListPopupWidget* OpenCommandListPopupWidget();

	UEditorTextInputPopupWidget* OpenTextInputPopupWidget(const FTextInputPopupParameter& InParam);

	TWeakPtr<FCardEditor> GetCardEditor() const
	{
		return CardEditor;
	}

private:

	void InitializeCardEditor();

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CardEditor")
	TSubclassOf<UCardEditorWidget> EditorWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CardEditor")
	TSubclassOf<UCommandListPopupWidget> CommandListPopupWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CardEditor")
	TSubclassOf<UEditorTextInputPopupWidget> EditorTextInputPopupWidgetClass;

private:

	UPROPERTY(Transient)
	UCardEditorWidget* CurrentCardEditor;

	TSharedPtr<FCardEditor> CardEditor;
};
