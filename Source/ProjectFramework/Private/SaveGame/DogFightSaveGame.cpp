// Dog Fight Game Code By CYM.


#include "SaveGame/DogFightSaveGame.h"

void UDogFightSaveGame::SetMoney(int32 InMoney)
{
	Money = InMoney;
}

UDogFightSaveGame::UDogFightSaveGame()
{
	SaveSlotName = TEXT("DefaultSaveSlot");
	UserIndex = 0;
	Money = 0;
}