local DamageTypeDef = require("DogFight.Services.DamageService.DamageTypeDef")

---@class DamageConfig : ServiceConfigBase
local DamageConfig = UnrealClass("DogFight.Services.ServiceConfigBase")

local Config = {
    [DamageTypeDef.Default] = {
        Name = "Default",
        DamageType = UE.UExtendedDamageType.StaticClass(),
        Script = "DogFight.DamageInstance.DefaultDamage",
        DisplayParams = {
            Color = {1, 0, 0},
        },
    },
    [DamageTypeDef.Explosion] = {
        Name = "Explosion"
    }
}

function DamageConfig:GetData()
    return Config
end

return DamageConfig