local CardActionCommand = require "Card.CardCommand.CardActionCommand"
local ProjectileNameDef = require "DogFight.Services.ProjectileService.ProjectileNameDef"
local CardTargetHelper = require "Card.CardTarget.CardTargetHelper"
local CardCommandHelper = require "Card.CardCommand.CardCommandHelper"

---@field _ProjectileName string Name of projectile to launch.
---@field _MuzzleSpeed number Initial speed of projectile.
---@field _TargetList table Target list this command executing with.
---@field _AliveProjectileCount number Total count of alive projectiles.
---@field _DelegateHelperList table List of all delegate helper for projectile death.
---@class ActionLaunchProjectile : CardActionCommand Action that launch a projectile toward target.
local ActionLaunchProjectile = Class(CardActionCommand)

function ActionLaunchProjectile:OnInit()
    CardActionCommand.OnInit(self)

    self._AliveProjectileCount = 0
    self._DelegateHelperList = {}
end

---Setup the necessary info for this command.
---@param ProjectileInfo table Table of all projectile settings.
---@param TargetInfo table Target list for this command.
function ActionLaunchProjectile:SetCommandInfo(ProjectileInfo, TargetInfo)
    -- Read in all settings for projectile
    if ProjectileInfo then
        self._ProjectileName = ProjectileInfo.Name or ProjectileNameDef.DefaultProjectile
        self._MuzzleSpeed = ProjectileInfo.MuzzleSpeed or 100
    end

    -- Record all target information
    self._TargetList = TargetInfo
end

---Register the dead callback to specified projectile.
---@param self ActionLaunchProjectile Command instance.
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
---@param self ActionLaunchProjectile Command instance.
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

function ActionLaunchProjectile:StartCommand()
    CardActionCommand.StartCommand(self)

    local bFailed = false
    -- Check target count
    if not self._TargetList or #self._TargetList == 0 then
        bFailed = true
    end

    -- Check projectile settings
    if not self._ProjectileName then
        bFailed = true
    end

    if bFailed then
        self:FailedWithParams()
        return
    end

    --- Get projectile spawn location from character
    local Launcher
    local SpawnLoc
    ---@type ATopDownStylePlayerController
    local PlayerController = CardCommandHelper.GetOwnerPlayerController(self)
    if PlayerController then
        Launcher = PlayerController:GetCharacterPawn()
        if Launcher then
            SpawnLoc = Launcher:GetProjectileSpawnLocation()
        end
    end

    ---Iterate through the target list
    for _, v in ipairs(self._TargetList) do
        local TargetPos = CardTargetHelper.TargetInfoToPosition(v)
        if TargetPos then
            ---Spawn projectile instance by name
            ---@type ProjectileService
            local ProjectileService = GetGameService(self._CardLogic, GameServiceNameDef.ProjectileService)
            if ProjectileService then
                local NewProjectile = ProjectileService:SpawnProjectileByName(self._ProjectileName, SpawnLoc)
                if NewProjectile then
                    NewProjectile:SetLauncher(Launcher)
                    self._AliveProjectileCount = self._AliveProjectileCount + 1
                    RegisterCallbackToProjectile(self, NewProjectile)
                    NewProjectile:LaunchToTargetWithSpeed(TargetPos, self._MuzzleSpeed)
                end
            end
        end
    end
end

function ActionLaunchProjectile:OnProjectileDead(InProjectile)
    UnregisterCallbackFromProjectile(self, InProjectile)

    self._AliveProjectileCount = self._AliveProjectileCount - 1
    if self._AliveProjectileCount <= 0 then
        self:SuccessWithParams()
    end
end

function ActionLaunchProjectile:tostring()
    return "ActionLaunchProjectile"
end

return ActionLaunchProjectile