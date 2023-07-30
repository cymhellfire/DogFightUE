local ProjectileTypeDef = require "DogFight.Services.ProjectileService.ProjectileTypeDef"

---@class BuffLuckyBomb : BuffLogicBase A lucky tester bomb bind to target character.
---@field _bFinishing boolean Indicate if this buff is in finish progress.
---@field _bChecking boolean Indicate if this buff is in buff check progress.
---@field _Character ATopDownStylePlayerCharacter Character that carrying this buff.
---@field _Bomb ANewProjectileBase Bomb projectile instance.
---@field _DetonateRate number Chance of the bomb detonate every time applied.
---@field _DelegateHelper DelegateHelper Helper for listening bomb dead event.
---@field _Damage number Damage apply when bomb detonate.
---@field _DamageRadius number Damage radius of bomb.
local BuffLuckyBomb = UnrealClass("DogFight.Buff.BuffLogic.BuffLogicBase")

local SpawnOffset = 350

---Register callback for dead event of projectile
---@param self BuffLuckyBomb Buff instance.
---@param InProjectile ANewProjectileBase Projectile instance.
local function RegisterCallbackToProjectile(self, InProjectile)
    if not InProjectile then
        return
    end

    ---@type DelegateHelperService
    local DelegateHelperService = GetGameService(self._Owner, GameServiceNameDef.DelegateHelperService)
    if DelegateHelperService then
        self._DelegateHelper = DelegateHelperService:BindCallback(InProjectile.OnProjectileDead, self, 
            self.OnBombDead)
    end
end

---@param self BuffLuckyBomb Buff instance.
---@param InProjectile ANewProjectileBase Projectile instance.
local function UnRegisterCallbackFromProjectile(self, InProjectile)
    if not self._DelegateHelper then
        return
    end

    ---@type DelegateHelperService
    local DelegateHelperService = GetGameService(self._Owner, GameServiceNameDef.DelegateHelperService)
    if DelegateHelperService then
        DelegateHelperService:ReleaseDelegateHelper(self._DelegateHelper)
        self._DelegateHelper = nil
    end
end

---@param InBomb ANewProjectileBase
local function MarkBombTriggered(InBomb)
    if not InBomb then
        return
    end

    -- Make the bomb explode on next collision
    InBomb.bDeadWhenStop = true
    InBomb.MovementComponent.bShouldBounce = false
end

---@param self BuffLuckyBomb
---@return boolean Whether the bomb is triggered.
local function ChanceToDetonate(self)
    local Num = math.random(0, 1)
    -- Detonate bomb
    if Num <= self._DetonateRate and self._Bomb then
        MarkBombTriggered(self._Bomb)
        return true
    end

    return false
end

---@param self BuffLuckyBomb
---@param Target ATopDownStylePlayerCharacter
local function MoveBombToTarget(self, Target)
    if Target and self._Bomb then
        local SpawnLoc = Target:K2_GetActorLocation()
        SpawnLoc.Z = SpawnLoc.Z + SpawnOffset

        self._Bomb:K2_SetActorLocation(SpawnLoc, false, nil, true)
        self._Bomb:LaunchToTargetWithSpeed(SpawnLoc, 1)
    end
end

function BuffLuckyBomb:OnInit()
    self.Super.OnInit(self)
    self._bFinishing = false
    self._bChecking = false
    self._bOnInit = true

    -- Add follow mod
    self:AddMod("BuffModFollowActivePlayer")
end

function BuffLuckyBomb:OnSetupArgument()
    if self._BuffArgs then
        self._DetonateRate = self._BuffArgs.BuffRatio or 0.35
        self._Damage = self._BuffArgs.Damage or 10
        self._DamageRadius = self._BuffArgs.DamageRadius or 250

        -- Spawn bomb instance
        ---@type ProjectileService
        local ProjectileService = GetGameService(self._Owner, GameServiceNameDef.ProjectileService)
        if ProjectileService then
            self._Bomb = ProjectileService:SpawnProjectileAtPos(ProjectileTypeDef.LuckyBomb, UE.FVector(), UE.FRotator())
            self._Bomb:LaunchToTargetWithSpeed(UE.FVector(), 1)
            self._Bomb.Damage = self._Damage
            self._Bomb.WarheadData.DamageRadius = self._DamageRadius

            -- Register callback
            RegisterCallbackToProjectile(self, self._Bomb)
        end
    else
        error("BuffLuckyBomb:OnSetupArgument Invalid _BuffArgs")
    end
end

---@param InCharacter ATopDownStylePlayerCharacter
function BuffLuckyBomb:OnApply(InCharacter)
    self.Super.OnApply(self, InCharacter)

    -- Record character
    self._Character = InCharacter

    -- Move to target for first time
    if self._bOnInit then
        self._bOnInit = nil
        MoveBombToTarget(self, self._Character)
    end
end

---@param InCharacter ATopDownStylePlayerCharacter
function BuffLuckyBomb:OnRemove(InCharacter)
    self.Super.OnRemove(self, InCharacter)

    -- Clear character
    self._Character = nil
end

---@type EBuffCheckType
function BuffLuckyBomb:DoCheck(InType)
    if InType == UE.EBuffCheckType.PrePlayerRound then
        -- Move bomb instance above current character
        MoveBombToTarget(self, self._Character)

        -- Check if trigger the bomb this time
        if ChanceToDetonate(self) then
            -- Finish checking after bomb is detonated
            self._bChecking = true
            return
        end
    end

    self._Owner:FinishDoCheck()
end

function BuffLuckyBomb:OnFinish()
    self.Super.OnFinish(self)

    -- Mark this buff as finished
    self._bFinishing = true

    -- Detonate the bomb if it still alive
    if self._Bomb then
        self._Bomb:Detonate()
    end
end

---@param InProjectile ANewProjectileBase
function BuffLuckyBomb:OnBombDead(InProjectile)
    -- Remove callback
    UnRegisterCallbackFromProjectile(self, InProjectile)

    -- Reset changed setting
    self._Bomb.bDeadWhenStop = false
    self._Bomb.MovementComponent.bShouldBounce = true

    -- Clear the reference
    self._Bomb = nil

    -- Finish check
    if self._bChecking then
        self._Owner:FinishDoCheck()
    end

    -- Remove this buff
    if not self._bFinishing then
        self._Owner:Finish()
    end
end

function BuffLuckyBomb:tostring()
    return "BuffLuckyBomb"
end

return BuffLuckyBomb