require "UnLua"

---@field CurProjectileId number Current previewing projectile id.
---@field ProjectileSpawnMethod function Function for spawning preview projectiles.
---@class UtilSceneController : BP_Ctrl_UtilsScene_C
local UtilSceneController = Class()

---Default mode that spawn projectile at origin and launch toward target.
---@param self UtilSceneController Player controller instance.
---@param InPos FVector Target position.
local function SpawnProjectileDefault(self, InPos)
    if not self.CurProjectileId then
        return
    end

    ---@type ProjectileService
    local ProjectileService = GetGameService(GameServiceNameDef.ProjectileService)
    if ProjectileService then
        local NewProjectile = ProjectileService:SpawnProjectileAtPos(self.CurProjectileId, self:K2_GetActorLocation(), self:K2_GetActorRotation())
        -- Launch new projectile to target
        if NewProjectile then
            local FinalPos = UE.FVector(InPos.X, InPos.Y, InPos.Z + 200)
            NewProjectile:LaunchToTargetWithSpeed(FinalPos, 1000)
        end
    end
end

---Drop mode that spawn projectile above target position and let it fall freely.
---@param self UtilSceneController Player controller instance.
---@param InPos FVector Target position.
local function SpawnProjectileDrop(self, InPos)
    if not self.CurProjectileId then
        return
    end

    ---@type ProjectileService
    local ProjectileService = GetGameService(GameServiceNameDef.ProjectileService)
    if ProjectileService then
        local SpawnPos = UE.FVector(InPos.X, InPos.Y, InPos.Z + 1000)
        local NewProjectile = ProjectileService:SpawnProjectileAtPos(self.CurProjectileId, SpawnPos, UE.FRotator(0, 0, 0))
        -- Move the projectile slightly
        if NewProjectile then
            NewProjectile:LaunchToTargetWithSpeed(SpawnPos, 1)
        end
    end
end

---Projectile spawn mode table.
local ProjectileSpawnMode = {
    UI_ProjectileSpawnMode_Default = SpawnProjectileDefault,
    UI_ProjectileSpawnMode_Drop = SpawnProjectileDrop,
}

function UtilSceneController:ReceiveBeginPlay()
    -- Initial spawn mode
    self.ProjectileSpawnMethod = SpawnProjectileDefault
end

---Set the projectile to spawn.
---@param InId number Id of projectile.
function UtilSceneController:SetProjectileId(InId)
    self.CurProjectileId = InId
end

---Set the projectile spawn mode.
---@param InMode string New mode to spawn projectiles.
function UtilSceneController:SetProjectileSpawnMode(InMode)
    if ProjectileSpawnMode[InMode] then
        self.ProjectileSpawnMethod = ProjectileSpawnMode[InMode]
    end
end

function UtilSceneController:OnTargetClicked(InPos)
    print("OnTargetClicked: Click at [" .. InPos.X .. "," .. InPos.Y .. "," .. InPos.Z .. "]")
    if self.ProjectileSpawnMethod then
        self.ProjectileSpawnMethod(self, InPos)
    end
end

local BindKey = UnLua.Input.BindKey

BindKey(UtilSceneController, "LeftMouseButton", "Pressed", function(self, Key)
    -- Get hit info from cursor position
    local bHit, HitResult = self:GetHitResultUnderCursor(UE.ECollisionChannel.ECC_Visibility, false)
    if bHit and HitResult.bBlockingHit then
        self:OnTargetClicked(HitResult.Location)
    end
end)

return UtilSceneController