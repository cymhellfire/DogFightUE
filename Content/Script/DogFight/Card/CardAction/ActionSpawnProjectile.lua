local CardActionCommand = require "Card.CardCommand.CardActionCommand"
local CardTargetHelper = require "Card.CardTarget.CardTargetHelper"
local CardCommandHelper = require "Card.CardCommand.CardCommandHelper"

---@class ActionSpawnProjectile : CardActionCommand
---@field _DelegateHelperList table List of all delegate helper for projectile death.
---@field _AliveProjectileCount number Total count of alive projectiles.
local ActionSpawnProjectile = UnrealClass(CardActionCommand)

---@param ProjectileInfo table Table of all projectile settings.
---@param TargetInfo table Target list for this command.
function ActionSpawnProjectile:SetProjectileInfo(ProjectileInfo, TargetInfo)
    -- Read in all settings for projectile
    self._ProjectileInfo = self:ConvertArgumentTable(ProjectileInfo)

    -- Record all target information
    self._TargetList = TargetInfo
end

---Register the dead callback to specified projectile.
---@param self ActionSpawnProjectile Command instance.
---@param Projectile ProjectileBase Projectile instance to register with.
local function RegisterCallbackToProjectile(self, Projectile)
    ---@type DelegateHelperService
    local DelegateHelperService = GetGameService(self._CardLogic, GameServiceNameDef.DelegateHelperService)
    if DelegateHelperService then
        local NewHelper = DelegateHelperService:BindCallback(Projectile.OnProjectileDead, self, self.OnProjectileDead)
        self._DelegateHelperList[Projectile] = NewHelper
    end
end

---Remove the dead callback from specified projectile.
---@param self ActionSpawnProjectile Command instance.
---@param Projectile ProjectileBase Projectile instance to remove with.
local function UnregisterCallbackFromProjectile(self, Projectile)
    local Helper = self._DelegateHelperList[Projectile]
    if not Helper then
        return
    end

    ---@type DelegateHelperService
    local DelegateHelperService = GetGameService(self._CardLogic, GameServiceNameDef.DelegateHelperService)
    if DelegateHelperService then
        DelegateHelperService:ReleaseDelegateHelper(Helper)
    end

    self._DelegateHelperList[Projectile] = nil
end

---Command executing function
function ActionSpawnProjectile:StartCommand()
    CardActionCommand.StartCommand(self)

    local bFailed = false
    -- Check target count
    if not self._TargetList or #self._TargetList == 0 then
        bFailed = true
    end

    -- Check projectile settings
    if not self._ProjectileInfo.ProjectileId then
        bFailed = true
    end

    if bFailed then
        self:FailedWithParams()
        return
    end

    local Launcher
    ---@type ATopDownStylePlayerController
    local PlayerController = CardCommandHelper.GetOwnerPlayerController(self)
    if PlayerController then
        Launcher = PlayerController:GetCharacterPawn()
    end

    self._AliveProjectileCount = 0
    self._DelegateHelperList = {}

    ---@type ProjectileService
    local ProjectileService = GetGameService(self._CardLogic, GameServiceNameDef.ProjectileService)
    if ProjectileService then
        -- Handle each target
        for _, v in ipairs(self._TargetList) do
            local TargetPos = CardTargetHelper.TargetInfoToPosition(v)
            if TargetPos then
                -- Construct spawn position
                local SpawnPos = UE.FVector()
                SpawnPos.X = TargetPos.X
                SpawnPos.Y = TargetPos.Y
                SpawnPos.Z = TargetPos.Z + self._ProjectileInfo.SpawnHeight

                ---Spawn projectile instance by name
                local Projectile = ProjectileService:SpawnProjectileAtPos(self._ProjectileInfo.ProjectileId, SpawnPos, UE.FRotator(0,0,0))
                if Projectile then
                    Projectile:SetLauncher(Launcher)
                    if type(self._ProjectileInfo.Damage) == "number" then
                        Projectile.Damage = self._ProjectileInfo.Damage
                    end
                    if type(self._ProjectileInfo.DamageRadius) == "number" then
                        Projectile.WarheadData.DamageRadius = self._ProjectileInfo.DamageRadius
                    end
                    self._AliveProjectileCount = self._AliveProjectileCount + 1
                    RegisterCallbackToProjectile(self, Projectile)
                    -- Launch with slight move
                    Projectile:LaunchToTargetWithSpeed(TargetPos, 1)
                end
            end
        end
    end

    self:SuccessWithParams()
end

---@param InProjectile ANewProjectileBase
function ActionSpawnProjectile:OnProjectileDead(InProjectile)
    UnregisterCallbackFromProjectile(self, InProjectile)

    self._AliveProjectileCount = self._AliveProjectileCount - 1
    if self._AliveProjectileCount <= 0 then
        self:SuccessWithParams()
    end
end

function ActionSpawnProjectile:tostring()
    return "ActionSpawnProjectile"
end

return ActionSpawnProjectile
