local GameLuaStateNameDef = require("DogFight.Services.GameStateMachineService.GameLuaStateNameDef")

---@class GameLuaStateConfig : ServiceConfigBase Table contains all lua game state configurations.
local GameLuaStateConfig = UnrealClass("DogFight.Services.ServiceConfigBase")
local GameWidgetNameDef = require("DogFight.Services.GameWidgetService.GameWidgetNameDef")

local Config = {
    [GameLuaStateNameDef.StateMainMenu] = {
        LoadMap = "/Game/DogFightGame/Level/MainMenu/MainMenu",
        Script = "DogFight.Services.GameStateMachineService.GameLuaState",
        InitialWidgets = {
            GameWidgetNameDef.WidgetMainMenu,
        },
    },
    [GameLuaStateNameDef.StatePreparationRoom] = {
        Script = "DogFight.Services.GameStateMachineService.States.StatePreparationRoom",
        InitialWidgets = {
            GameWidgetNameDef.WidgetPreparationRoom,
        },
    },
    [GameLuaStateNameDef.StateGameLobby] = {
        Script = "DogFight.Services.GameStateMachineService.States.StateGameLobby",
        InitialWidgets = {
            GameWidgetNameDef.WidgetGameLobby,
        }
    },
}

function GameLuaStateConfig:GetData()
    return Config
end

return GameLuaStateConfig