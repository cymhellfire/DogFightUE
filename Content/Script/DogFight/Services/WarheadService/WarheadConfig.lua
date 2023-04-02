require "UnLua"
local WarheadNameDef = require("DogFight.Services.WarheadService.WarheadNameDef")
local DamageNameDef = require("DogFight.Services.DamageService.DamageNameDef")

---@class WarheadConfig : ServiceConfigBase Warhead config table.
local WarheadConfig = Class("DogFight.Services.ServiceConfigBase")

local Config = {
    [WarheadNameDef.Default] = {
        Id = 0,
        EffectId = 0,
        DamageType = DamageNameDef.Default,
    },
    [WarheadNameDef.Nuke] = {
        Id = 1,
        EffectId = 1,
        DamageType = DamageNameDef.Explosion,
        DamageRadius = 1000,
    },
}

function WarheadConfig:GetData()
    return Config
end

return WarheadConfig