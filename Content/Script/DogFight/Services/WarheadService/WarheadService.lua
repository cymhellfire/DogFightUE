require "UnLua"

---@class WarheadService Service that hold all warhead data.
local WarheadService = Class("DogFight.Services.GameServiceBase")
local WarheadConfig = require("DogFight.Services.WarheadService.WarheadConfig")
local WarheadNameDef = require("DogFight.Services.WarheadService.WarheadNameDef")

---Setup the warhead information of specified projectile.
---@param InProjectile AProjectileBase Projectile instance.
---@param WarheadName string Name of warhead config
function WarheadService:SetWarheadInfoForProjectile(InProjectile, WarheadName)
    if type(WarheadName) ~= "string" then
        return
    end

    local Config = WarheadConfig[WarheadName]
    if Config then
        InProjectile.WarheadId = Config.Id or -1
        InProjectile.WarheadData.WarheadName = Config.Name or ""
        InProjectile.WarheadData.GameEffectId = Config.EffectId or -1
        InProjectile.WarheadData.bUseProjectileRotation = Config.EffectUseProjectileRotation or false
        InProjectile.WarheadData.DamageTypeName = Config.DamageType or ""
    end
end

return WarheadService