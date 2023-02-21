require "UnLua"
local DamageTypeNameDef = require("DogFight.Services.DamageTypeService.DamageTypeNameDef")

---@class DamageTypeConfig : ServiceConfigBase
local DamageTypeConfig = Class("DogFight.Services.ServiceConfigBase")

local Config = {
    [DamageTypeNameDef.Default] = {
        Id = 0,
    },
    [DamageTypeNameDef.Explosion] = {
        Id = 1,
    }
}

function DamageTypeConfig:GetData()
    return Config
end

return DamageTypeConfig