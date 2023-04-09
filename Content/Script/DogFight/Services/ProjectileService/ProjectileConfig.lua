require "UnLua"
local ProjectileNameDef = require("DogFight.Services.ProjectileService.ProjectileNameDef")
local WarheadNameDef = require("DogFight.Services.WarheadService.WarheadNameDef")

---@class ProjectileConfig : ServiceConfigBase Class manage all projectile configurations.
local ProjectileConfig = Class("DogFight.Services.ServiceConfigBase")

---Config data
local Config = {
    [ProjectileNameDef.DefaultProjectile] = {
        Id = 1,
        ResPath = "/Game/DogFightGame/Blueprints/Projectile/BP_Proj_Default",
        ScriptPath = "DogFight.Projectiles.ProjectileLogic.DefaultLogic",
        Warhead = WarheadNameDef.Default,
    },
    [ProjectileNameDef.BouncyProjectile] = {
        Id = 2,
        ResPath = "/Game/DogFightGame/Blueprints/Projectile/BP_Proj_Bouncy",
        ScriptPath = "",
        Warhead = WarheadNameDef.Nuke,
    },
    [ProjectileNameDef.HomingProjectile] = {
        Id = 3,
        ResPath = "/Game/DogFightGame/Blueprints/Projectile/BP_Proj_Homing",
        ScriptPath = "",
        Warhead = WarheadNameDef.Default,
        Damage = 50,
    },
    [ProjectileNameDef.PeriodHomingProjectile] = {
        Id = 4,
        ResPath = "/Game/DogFightGame/Blueprints/Projectile/BP_Proj_PeriodHoming",
        Warhead = WarheadNameDef.Default,
    }
}

function ProjectileConfig:GetData()
    return Config
end

return ProjectileConfig