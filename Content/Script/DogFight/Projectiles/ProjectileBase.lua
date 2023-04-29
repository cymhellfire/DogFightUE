---@class ProjectileBase : ANewProjectileBase
local ProjectileBase = UnrealClass()

---Setup logic script for this projectile.
---@param InPath string Path of script file.
function ProjectileBase:SetProjectileScriptModule(InPath)
    if type(InPath) ~= "string" or #InPath == 0 then
        return
    end

    ---@type ProjectileLogicBase
    local Module = require(InPath)
    if Module then
        self.Logic = Module:New()
        self.Logic:InitWithOwner(self)
    end
end

function ProjectileBase:ReceiveTick(DeltaTime)
    if self.Logic and self.Logic.Tick then
        self.Logic:Tick(DeltaTime)
    end
end

return ProjectileBase
