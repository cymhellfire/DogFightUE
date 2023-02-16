require "UnLua"

---@class ProjectileService : UProjectileService Service that handle in-game projectiles operations.
local ProjectileService = Class("DogFight.Services.GameServiceBase")
local ProjectileConfig = require("DogFight.Services.ProjectileService.ProjectileConfig")

function ProjectileService:GatherProjectileConfigData()
    local Result = UE.TArray(UE.FProjectileConfigData)
    for k, v in pairs(ProjectileConfig.Data) do
        local NewConfig = UE.FProjectileConfigData()
        NewConfig.ProjectileId = v.Id
        NewConfig.ResourcePath = v.ResPath
        NewConfig.ScriptPath = v.ScriptPath

        Result:Add(NewConfig)
    end
    return Result
end

function ProjectileService:GetDefineNameOfProjectile(Id)
    return ProjectileConfig:ProjectileIdToName(Id)
end

---Spawn projectile by name.
---@param InName string Name of projectile to spawn.
---@param InPos FVector Position to place new projectile.
---@param InRot FRotator Initial rotation of new projectile.
---@return ProjectileBase New spawned projectile.
function ProjectileService:SpawnProjectileByName(InName, InPos, InRot)
    local ProjectileId = ProjectileConfig:ProjectileNameToId(InName)
    if ProjectileId then
        return self:SpawnProjectileAtPos(ProjectileId, InPos, InRot)
    end
end

return ProjectileService