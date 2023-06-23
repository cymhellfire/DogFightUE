local CardActionCommand = require "Card.CardCommand.CardActionCommand"
local ProjectileTypeDef = require "DogFight.Services.ProjectileService.ProjectileTypeDef"
local CardTargetHelper = require "Card.CardTarget.CardTargetHelper"
local CardCommandHelper = require "Card.CardCommand.CardCommandHelper"

---@field _ProjectileId string Name of projectile to launch.
---@field _MuzzleSpeed number Initial speed of projectile.
---@field _ProjectileDamage number Projectile damage.
---@field _TargetList table Target list this command executing with.
---@field _AliveProjectileCount number Total count of alive projectiles.
---@field _DelegateHelperList table List of all delegate helper for projectile death.
---@class ActionLaunchProjectile : CardActionCommand Action that launch a projectile toward target.
local ActionLaunchProjectile = UnrealClass(CardActionCommand)

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
        self._ProjectileId = ProjectileInfo.Id or ProjectileTypeDef.DefaultProjectile
        self._MuzzleSpeed = ProjectileInfo.MuzzleSpeed or 100
        self._ProjectileDamage = ProjectileInfo.Damage or nil
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
    if not self._ProjectileId then
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
                local NewProjectile = ProjectileService:SpawnProjectileAtPos(self._ProjectileId, SpawnLoc, UE.FRotator(0,0,0))
                if NewProjectile then
                    NewProjectile:SetLauncher(Launcher)
                    if type(self._ProjectileDamage) == "number" then
                        NewProjectile.Damage = self._ProjectileDamage
                    end
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