require "UnLua"

---@field CurProjectileId number Current previewing projectile id.
---@field ProjectileSpawnMethod function Function for spawning preview projectiles.
---@field DefaultModeSpawnHeight number Projectile spawn height offset for default mode.
---@field DefaultModeTargetHeight number Target height offset for default mode.
---@field DropModeSpawnHeight number Projectile spawn height offset for drop mode.
---@field AimModeSpawnHeight number Projectile spawn height offset for aim mode.
---@field ProjectileTarget AActor Projectile target for aim mode.
---@field DirectionalLight ADirectionalLight Directional light of current scene.
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

---Aim mode that use a moving object as target.
---@param self UtilSceneController Player controller instance.
---@param InPos FVector Target position.
local function SpawnProjectileAim(self, InPos)
    if not self.CurProjectileId then
        return
    end

    ---@type ProjectileService
    local ProjectileService = GetGameService(GameServiceNameDef.ProjectileService)
    if ProjectileService then
        local MyPos = self:K2_GetActorLocation()
        local SpawnPos = UE.FVector(MyPos.X, MyPos.Y, MyPos.Z + self.AimModeSpawnHeight)
        local NewProjectile = ProjectileService:SpawnProjectileAtPos(self.CurProjectileId, SpawnPos, UE.FRotator(0, 0, 0))
        -- Let the projectile seek to target
        if NewProjectile and self.ProjectileTarget then
            NewProjectile:HomingToTargetWithSpeed(self.ProjectileTarget, 1000)
        end
    end
end

---Projectile spawn mode table.
local ProjectileSpawnMode = {
    UI_ProjectileSpawnMode_Default = SpawnProjectileDefault,
    UI_ProjectileSpawnMode_Drop = SpawnProjectileDrop,
    UI_ProjectileSpawnMode_Aim = SpawnProjectileAim,
}

function UtilSceneController:ReceiveBeginPlay()
    -- Initial spawn mode
    self.ProjectileSpawnMethod = SpawnProjectileDefault

    self.DefaultModeSpawnHeight = 50
    self.DefaultModeTargetHeight = 50
    self.DropModeSpawnHeight = 100
    self.AimModeSpawnHeight = 100

    -- Get the directional light
    self.DirectionalLight = UE.UGameplayStatics.GetActorOfClass(self, UE.ADirectionalLight.StaticClass())

    -- Get the projectile target actor
    local AllActor = UE.UGameplayStatics.GetAllActorsOfClass(self, UE.AStaticMeshActor.StaticClass())
    local ActorList = AllActor:ToTable()
    for _, v in ipairs(ActorList) do
        local ObjectName = v:GetName()
        if string.find(ObjectName, "ProjectileTarget") then
            self.ProjectileTarget = v
            print("Find projectile target.")
            break
        end
    end
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

function UtilSceneController:SetAimModeSpawnHeight(InValue)
    self.AimModeSpawnHeight = InValue
end

function UtilSceneController:OnTargetClicked(InPos)
    print("OnTargetClicked: Click at [" .. InPos.X .. "," .. InPos.Y .. "," .. InPos.Z .. "]")
    if self.ProjectileSpawnMethod then
        self.ProjectileSpawnMethod(self, InPos)
    end
end

function UtilSceneController:SetDirectionalLightIntensity(InValue)
    if self.DirectionalLight then
        ---@type UDirectionalLightComponent
        local Light = self.DirectionalLight:GetComponentByClass(UE.UDirectionalLightComponent.StaticClass())
        if Light then
            Light:SetIntensity(InValue)
        end
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