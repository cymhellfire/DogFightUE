require "UnLua"

---@class WarheadService : GameServiceBase Service that hold all warhead data.
local WarheadService = Class("DogFight.Services.GameServiceBase")
local WarheadNameDef = require("DogFight.Services.WarheadService.WarheadNameDef")

function WarheadService:GetConfigPath()
    return "DogFight.Services.WarheadService.WarheadConfig"
end

---Setup the warhead information of specified projectile.
---@param InProjectile AProjectileBase Projectile instance.
---@param WarheadName string Name of warhead config
function WarheadService:SetWarheadInfoForProjectile(InProjectile, WarheadName)
    if type(WarheadName) ~= "string" then
        return
    end

    local Config = self.Config:GetConfigByName(WarheadName)
    if Config then
        InProjectile.WarheadId = Config.Id or -1
        InProjectile.WarheadData.WarheadName = Config.Name or ""
        InProjectile.WarheadData.GameEffectId = Config.EffectId or -1
        InProjectile.WarheadData.bUseProjectileRotation = Config.EffectUseProjectileRotation or false
        local DamageType = Config.DamageType or ""
        InProjectile.WarheadData.DamageTypeName = DamageType
        ---@type DamageService
        local DamageService = GetGameService(GameServiceNameDef.DamageService)
        if DamageService then
            InProjectile.WarheadData.DamageId = DamageService.Config:NameToId(DamageType) or -1
        end
        InProjectile.WarheadData.DamageRadius = Config.DamageRadius or 0
    end
end

return WarheadService