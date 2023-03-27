local WidgetNameDef = require("DogFight.Services.Config.GameWidgetNameDef")

---@class GameWidgetConfig Data table contains all game widgets define.
local GameWidgetConfig = {
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
}

return GameWidgetConfig