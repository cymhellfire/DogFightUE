local GameLuaStateNameDef = require("DogFight.Services.GameStateMachineService.GameLuaStateNameDef")

---@class GameLuaStateConfig : ServiceConfigBase Table contains all lua game state configurations.
local GameLuaStateConfig = UnrealClass("DogFight.Services.ServiceConfigBase")

local Config = {
    [GameLuaStateNameDef.StateMainMenu] = {
        Script = "DogFight.Services.GameStateMachineService.States.StateMainMenu",
        LoadMap = "/Game/DogFightGame/Level/MainMenu/MainMenu.MainMenu",
    },
}

function GameLuaStateConfig:GetData()
    return Config
end

return GameLuaStateConfig