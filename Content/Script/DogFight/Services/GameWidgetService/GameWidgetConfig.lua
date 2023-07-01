local WidgetNameDef = require("DogFight.Services.GameWidgetService.GameWidgetNameDef")

---@class GameWidgetConfig Data table contains all game widgets define.
local GameWidgetConfig = {
    [WidgetNameDef.WidgetCreateLobbyDialog] = {
        Path = "/Game/DogFightGame/UI/MainMenu/BP_Widget_CreateLobbyDialog"
    },
    [WidgetNameDef.WidgetLobbyBrowser] = {
        Path = "/Game/DogFightGame/UI/MainMenu/BP_Widget_LobbyBrowser",
    },
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
}

return GameWidgetConfig