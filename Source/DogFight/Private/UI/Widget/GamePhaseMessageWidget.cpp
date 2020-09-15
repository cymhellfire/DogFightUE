// Dog Fight Game Code By CYM.


#include "GamePhaseMessageWidget.h"

void UGamePhaseMessageWidget::SetGamePhase(const FName& GamePhase)
{
	// Invoke Blueprint implementation
	OnGamePhaseMessageChanged(GamePhase);
}

void UGamePhaseMessageWidget::SetCountdownContent(const FString& NewContext)
{
	// Skip non changed context
	if (NewContext == CountdownContent)
	{
		return;
	}

	CountdownContent = NewContext;

	// Invoke Blueprint implementation
	OnCountdownContentChanged();
}
