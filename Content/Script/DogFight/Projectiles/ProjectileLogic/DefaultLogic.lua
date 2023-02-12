require "UnLua"

---@class DefaultLogic : ProjectileLogicBase Logic of default projectile.
local DefaultLogic = Class("DogFight.Projectiles.ProjectileLogicBase")

function DefaultLogic:InitWithOwner(InOwner)
    self.Super.InitWithOwner(InOwner)

    print("Default logic start.")
end

return DefaultLogic