require "UnLua"
local DamageNameDef = require("DogFight.Services.DamageService.DamageNameDef")

---@class DamageConfig : ServiceConfigBase
local DamageConfig = Class("DogFight.Services.ServiceConfigBase")

local Config = {
    [DamageNameDef.Default] = {
        Id = 0,
        DamageType = UE.UExtendedDamageType.StaticClass(),
        Script = "DogFight.DamageInstance.DefaultDamage"
    },
    [DamageNameDef.Explosion] = {
        Id = 1,
    }
}

function DamageConfig:GetData()
    return Config
end

return DamageConfig