local ProjectileTypeDef = require("DogFight.Services.ProjectileService.ProjectileTypeDef")
local WarheadTypeDef = require("DogFight.Services.WarheadService.WarheadTypeDef")

---@class ProjectileConfig : ServiceConfigBase Class manage all projectile configurations.
local ProjectileConfig = UnrealClass("DogFight.Services.ServiceConfigBase")

---Config data
local Config = {
    [ProjectileTypeDef.DefaultProjectile] = {
        Name = "DefaultProjectile",
        ResPath = "/Game/DogFightGame/Blueprints/Projectile/BP_Proj_Default",
        ScriptPath = "DogFight.Projectiles.ProjectileLogic.DefaultLogic",
        Warhead = WarheadTypeDef.Default,
    },
    [ProjectileTypeDef.Fireball] = {
        Name = "Fireball",
        ResPath = "/Game/DogFightGame/Blueprints/Projectile/BP_Proj_Fireball",
        ScriptPath = "",
        Warhead = WarheadTypeDef.Fireball,
        Damage = 10,
    },
    [ProjectileTypeDef.HomingProjectile] = {
        Name = "HomingProjectile",
        ResPath = "/Game/DogFightGame/Blueprints/Projectile/BP_Proj_Homing",
        ScriptPath = "",
        Warhead = WarheadTypeDef.Default,
        Damage = 50,
    },
    [ProjectileTypeDef.PeriodHomingProjectile] = {
        Name = "PeriodHomingProjectile",
        ResPath = "/Game/DogFightGame/Blueprints/Projectile/BP_Proj_PeriodHoming",
        Warhead = WarheadTypeDef.Default,
    },
    [ProjectileTypeDef.LuckyBomb] = {
        Name = "LuckyBomb",
        ResPath = "/Game/DogFightGame/Blueprints/Projectile/BP_Proj_LuckyBomb",
        Warhead = WarheadTypeDef.Default,
    },
}

function ProjectileConfig:GetData()
    return Config
end

return ProjectileConfig