---@class DefaultLogic : ProjectileLogicBase Logic of default projectile.
local DefaultLogic = UnrealClass("DogFight.Projectiles.ProjectileLogicBase")

function DefaultLogic:InitWithOwner(InOwner)
    self.Super.InitWithOwner(InOwner)

    print("Default logic start.")
end

return DefaultLogic