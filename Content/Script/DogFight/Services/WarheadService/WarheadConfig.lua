local WarheadTypeDef = require("DogFight.Services.WarheadService.WarheadTypeDef")
local DamageTypeDef = require("DogFight.Services.DamageService.DamageTypeDef")

---@class WarheadConfig : ServiceConfigBase Warhead config table.
local WarheadConfig = UnrealClass("DogFight.Services.ServiceConfigBase")

local Config = {
    [WarheadTypeDef.Default] = {
        Name = "Default",
        EffectId = 0,
        DamageType = DamageTypeDef.Default,
    },
    [WarheadTypeDef.Nuke] = {
        Name = "Nuke",
        EffectId = 1,
        DamageType = DamageTypeDef.Explosion,
        DamageRadius = 1000,
    },
    [WarheadTypeDef.Fireball] = {
        Name = "Fireball",
        EffectId = 2,
        DamageType = DamageTypeDef.Default,
    }
}

function WarheadConfig:GetData()
    return Config
end

return WarheadConfig