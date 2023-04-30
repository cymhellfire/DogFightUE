---@class WarheadService : GameServiceBase Service that hold all warhead data.
local WarheadService = UnrealClass("DogFight.Services.GameServiceBase")
local WarheadTypeDef = require("DogFight.Services.WarheadService.WarheadTypeDef")

function WarheadService:GetConfigPath()
    return "DogFight.Services.WarheadService.WarheadConfig"
end

---Setup the warhead information of specified projectile.
---@param InProjectile AProjectileBase Projectile instance.
---@param WarheadId number Id of warhead config
function WarheadService:SetWarheadInfoForProjectile(InProjectile, WarheadId)
    if type(WarheadId) ~= "number" then
        return
    end

    local Config = self.Config:GetConfig(WarheadId)
    if Config then
        InProjectile.WarheadId = Config.Id or -1
        InProjectile.WarheadData.WarheadName = Config.Name or ""
        InProjectile.WarheadData.GameEffectId = Config.EffectId or -1
        InProjectile.WarheadData.bUseProjectileRotation = Config.EffectUseProjectileRotation or false
        local DamageType = Config.DamageType or ""
        InProjectile.WarheadData.DamageTypeName = DamageType
        ---@type DamageService
        local DamageService = GetGameService(self, GameServiceNameDef.DamageService)
        if DamageService then
            InProjectile.WarheadData.DamageId = DamageType or -1
        end
        InProjectile.WarheadData.DamageRadius = Config.DamageRadius or 0
    end
end

return WarheadService