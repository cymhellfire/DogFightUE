---@class StateMainMenu : GameLuaState Lua state for main menu.
local StateMainMenu = UnrealClass("DogFight.Services.GameStateMachineService.GameLuaState")

function StateMainMenu:OnEnter()
    self.Super.OnEnter(self)
end

function StateMainMenu:OnExit()
    self.Super.OnExit(self)
end

return StateMainMenu