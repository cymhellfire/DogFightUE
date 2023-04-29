---@field Projectile ProjectileBase Owner projectile instance
---@class ProjectileLogicBase Base class for all projectile logic scripts.
local ProjectileLogicBase = UnrealClass()

---Initialize this logic with owner projectile instance.
---@param InOwner ProjectileBase Owner projectile
function ProjectileLogicBase:InitWithOwner(InOwner)
    self.Projectile = InOwner
end

return ProjectileLogicBase