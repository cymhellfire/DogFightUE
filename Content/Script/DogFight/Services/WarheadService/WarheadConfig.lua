require "UnLua"
local WarheadNameDef = require("DogFight.Services.WarheadService.WarheadNameDef")

---@class WarheadConfig : ServiceConfigBase Warhead config table.
local WarheadConfig = Class("DogFight.Services.ServiceConfigBase")

local Config = {
    [WarheadNameDef.Default] = {
        Id = 0,
        EffectId = 0,
    },
    [WarheadNameDef.Nuke] = {
        Id = 1,
        EffectId = 1,
    },
}

function WarheadConfig:GetData()
    return Config
end

return WarheadConfig