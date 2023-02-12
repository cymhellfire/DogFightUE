require "UnLua"

---@field CurProjectileId number Current previewing projectile id.
---@field ProjectileSpawnMethod function Function for spawning preview projectiles.
---@field DefaultModeSpawnHeight number Projectile spawn height offset for default mode.
---@field DefaultModeTargetHeight number Target height offset for default mode.
---@field DropModeSpawnHeight number Projectile spawn height offset for drop mode.
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
        local MyPos = self:K2_GetActorLocation()
        local NewProjectile = ProjectileService:SpawnProjectileAtPos(self.CurProjectileId, 
            UE.FVector(MyPos.X, MyPos.Y, MyPos.Z + self.DefaultModeSpawnHeight), self:K2_GetActorRotation())
        -- Launch new projectile to target
        if NewProjectile then
            local FinalPos = UE.FVector(InPos.X, InPos.Y, InPos.Z + self.DefaultModeTargetHeight)
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
        local SpawnPos = UE.FVector(InPos.X, InPos.Y, InPos.Z + self.DropModeSpawnHeight)
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

    self.DefaultModeSpawnHeight = 50
    self.DefaultModeTargetHeight = 50
    self.DropModeSpawnHeight = 100
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

function UtilSceneController:SetDefaultModeSpawnHeight(InValue)
    self.DefaultModeSpawnHeight = InValue
end

function UtilSceneController:SetDefaultModeTargetHeight(InValue)
    self.DefaultModeTargetHeight = InValue
end

function UtilSceneController:SetDropModeSpawnHeight(InValue)
    self.DropModeSpawnHeight = InValue
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