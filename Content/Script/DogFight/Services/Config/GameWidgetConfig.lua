local WidgetNameDef = require("DogFight.Services.Config.GameWidgetNameDef")

---@class GameWidgetConfig Data table contains all game widgets define.
local GameWidgetConfig = {
    [WidgetNameDef.WidgetCardList] = {
        Path = "/Game/DogFightGame/UI/InGame/CardList/BP_Widget_CardList.BP_Widget_CardList",
    },
    [WidgetNameDef.WidgetTimeline] = {
        Path = "/Game/DogFightGame/UI/InGame/Timeline/BP_Widget_GameTimeline.BP_Widget_GameTimeline",
    },
    [WidgetNameDef.WidgetTest] = {
        Path = "/Game/DogFight/UI/GameLobby/BP_GameLobbyWidget.BP_GameLobbyWidget_C",
    },
}

return GameWidgetConfig