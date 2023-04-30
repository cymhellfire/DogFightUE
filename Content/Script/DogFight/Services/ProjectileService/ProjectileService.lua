---@class ProjectileService : GameServiceBase, UProjectileService Service that handle in-game projectiles operations.
local ProjectileService = UnrealClass("DogFight.Services.GameServiceBase")

function ProjectileService:GetConfigPath()
    return "DogFight.Services.ProjectileService.ProjectileConfig"
end

---Get description data of all projectile config.
---@return table All description data of projectiles.
function ProjectileService:GetAllProjectileUtilsDesc()
    local Result = {}
    for k, v in pairs(self.Config.Data) do
        local NewDesc = {}
        NewDesc.Id = v.Id
        NewDesc.DisplayName = v.Name

        Result[#Result + 1] = NewDesc
    end
    return Result
end

---Spawn new projectile instance of given config.
---@param InId number Id of projectile config.
---@return ANewProjectileBase New projectile instance.
function ProjectileService:SpawnNewProjectileInstance_Implementation(InId)
    local Config = self.Config:GetConfig(InId)
    if Config then
        local CurWorld = GetCurrentWorld(self)
        local ProjectileClass = LoadClass(Config.ResPath)
        if ProjectileClass and CurWorld then
            local SpawnTrans = UE.FTransform()
            ---@type ANewProjectileBase
            local NewProjectile = CurWorld:SpawnActorEx(ProjectileClass, SpawnTrans)

            -- Setup the warhead information
            if NewProjectile then
                ---@type WarheadService
                local WarheadService = GetGameService(self, GameServiceNameDef.WarheadService)
                if WarheadService then
                    WarheadService:SetWarheadInfoForProjectile(NewProjectile, Config.Warhead)
                end

                NewProjectile.Damage = Config.Damage
            end

            -- Setup the projectile logic script
            if NewProjectile then
                NewProjectile:SetProjectileScriptModule(Config.ScriptPath)
            end
            return NewProjectile
        end
    end
end

return ProjectileService