local BuffTypeDef = require "DogFight.Services.BuffService.BuffTypeDef"

---@class BuffConfig : ServiceConfigBase Config data of all character buff.
local BuffConfig = Class("DogFight.Services.ServiceConfigBase")

local Config = {
    [BuffTypeDef.AddPhysResist] = {
        Path = "DogFight.Buff.BuffLogic.AddPhysResistBuff",
    },
}

function BuffConfig:GetData()
    return Config
end

return BuffConfig