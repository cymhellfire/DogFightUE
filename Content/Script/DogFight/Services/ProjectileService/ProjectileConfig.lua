local ProjectileNameDef = require("DogFight.Services.ProjectileService.ProjectileNameDef")

---@field IdToNameCache table Cached map to convert projectile id to name.
---@class ProjectileConfig Class manage all projectile configurations.
local ProjectileConfig = {}

---Cache
ProjectileConfig.IdToNameCache = {}

---Config data
ProjectileConfig.Data = {
    [ProjectileNameDef.DefaultProjectile] = {
        Id = 1,
        ResPath = "/Game/DogFightGame/Blueprints/Projectile/BP_Proj_Default",
        ScriptPath = "DogFight.Projectiles.ProjectileLogic.DefaultLogic",
    }
}

---Convert a projectile name to corresponding id.
---@param InName string Projectile name to convert.
---@return number Id of projectile has the name.
function ProjectileConfig:ProjectileNameToId(InName)
    if self.Data[InName] then
        return self.Data[InName].Id
    end
end

---Convert a projectile id to corresponding name.
---@param InId number Projectile id to convert.
---@return string Name of projectile has the id.
function ProjectileConfig:ProjectileIdToName(InId)
    -- Use the cache first
    if self.IdToNameCache[InId] then
        return self.IdToNameCache[InId]
    end

    local Name = nil
    for k, v in pairs(self.Data) do
        if v.Id == InId then
            Name = k
            -- Cache the result
            self.IdToNameCache[InId] = k
            break
        end
    end

    return Name
end

return ProjectileConfig