local WidgetNameDef = require("DogFight.Services.GameWidgetService.GameWidgetNameDef")

---@class GameWidgetConfig Data table contains all game widgets define.
local GameWidgetConfig = {
    ---Common
    [WidgetNameDef.WidgetGenericMessageDialog] = {
        Path = "/Game/DogFightGame/UI/Common/BP_Widget_GenericMessageDialog",
    },
    [WidgetNameDef.WidgetJoiningSession] = {
        Path = "/Game/DogFightGame/UI/MainMenu/BP_Widget_JoiningSession",
    },
    [WidgetNameDef.WidgetCreateLobbyDialog] = {
        Path = "/Game/DogFightGame/UI/MainMenu/BP_Widget_CreateLobbyDialog",
    },
    ---Main Menu
    [WidgetNameDef.WidgetMainMenu] = {
        Path = "/Game/DogFightGame/UI/MainMenu/BP_Widget_MainMenu",
    },
    [WidgetNameDef.WidgetLobbyBrowser] = {
        Path = "/Game/DogFightGame/UI/MainMenu/BP_Widget_LobbyBrowser",
    },
    [WidgetNameDef.WidgetGameLoading] = {
        Path = "/Game/DogFightGame/UI/Loading/BP_Widget_GameLoading",
    },
    ---GameLobby
    [WidgetNameDef.WidgetGameLobby] = {
        Path = "/Game/DogFightGame/UI/GameLobby/BP_Widget_GameLobby",
    },
    ---InGame
    [WidgetNameDef.WidgetCardList] = {
        Path = "/Game/DogFightGame/UI/InGame/CardList/BP_Widget_CardList",
    },
    [WidgetNameDef.WidgetTimeline] = {
        Path = "/Game/DogFightGame/UI/InGame/Timeline/BP_Widget_GameTimeline",
    },
    [WidgetNameDef.WidgetDamageDisplay] = {
        Path = "/Game/DogFightGame/UI/InGame/DamageDisplay/BP_Widget_DamageDisplayCanvas",
    },
    [WidgetNameDef.WidgetCharacterState] = {
        Path = "/Game/DogFightGame/UI/InGame/PlayerCharacterState/BP_Widget_PlayerCharacterStateCanvas",
    },
    [WidgetNameDef.WidgetCharacterMenu] = {
        Path = "/Game/DogFightGame/UI/InGame/PlayerCharacterState/BP_Widget_PlayerCharacterDetailState"
    },
    [WidgetNameDef.WidgetPreparationRoom] = {
        Path = "/Game/DogFightGame/UI/PreparationRoom/BP_Widget_PreparationRoom"
    },
}

return GameWidgetConfig