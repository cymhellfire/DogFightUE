---@class StateMainMenu : GameLuaState Lua state for main menu.
local StateMainMenu = UnrealClass("DogFight.Services.GameStateMachineService.GameLuaState")
local GameWidgetNameDef = require("DogFight.Services.GameWidgetService.GameWidgetNameDef")

function StateMainMenu:OnEnter()
    self.Super.OnEnter(self)

    ---@type GameWidgetService
    local GameWidgetService = GetGameService(self.StateMachine, GameServiceNameDef.GameWidgetService)
    if GameWidgetService then
        GameWidgetService:ShowWidget(GameWidgetNameDef.WidgetMainMenu, true)
    end
end

return StateMainMenu