local CardLogicCommand = require "Card.CardCommand.CardLogicCommand"

---@field _TargetInfo table Target acquire settings.
---@field _ProjectileInfo table Projectile settings.
---@class LogicRandomDropProjectile : CardLogicCommand
local LogicRandomDropProjectile = UnrealClass(CardLogicCommand)

local CommandNameDef = {
    AcquireTarget = "AcquireTarget",
    SpawnProjectile = "SpawnProjectile",
}

---Initialize the card workflow
function LogicRandomDropProjectile:OnInit(InParam)
    CardLogicCommand.OnInit(self, InParam)

    -- Record projectile info
    self._ProjectileInfo = InParam.ProjectileInfo

    -- Register all commands
    local CommandTable = {
        [CommandNameDef.AcquireTarget] = {
            Script = "DogFight.Card.CardAction.ActionCardAcquireTarget",
            OnCreate = self.OnAcquireTargetCreated,
            OnFinish = self.OnAcquireTargetFinished,
        },
        [CommandNameDef.SpawnProjectile] = {
            Script = "DogFight.Card.CardAction.ActionSpawnProjectile",
            OnCreate = self.OnSpawnProjectileCreated,
            OnFinish = self.OnSpawnProjectileFinished,
        },
    }
    self:RegisterCommandTable(CommandTable)
end

---Start executing
function LogicRandomDropProjectile:StartCommand()
    CardLogicCommand.StartCommand(self)

    self:RunCommand(CommandNameDef.AcquireTarget)
end

---@param InCommand ActionCardAcquireTarget
function LogicRandomDropProjectile:OnAcquireTargetCreated(InCommand)
    if self._TargetInfo then
        InCommand:InitAcquireSettings(self._TargetInfo)
    else
        self:FailedWithParams()
    end
end

---Command finish callback
function LogicRandomDropProjectile:OnAcquireTargetFinished(Result, TargetInfo)
    self._TargetInfo = TargetInfo

    self:RunCommand(CommandNameDef.SpawnProjectile)
end

---Command create callback
---@param InCommand ActionSpawnProjectile
function LogicRandomDropProjectile:OnSpawnProjectileCreated(InCommand)
    InCommand:SetProjectileInfo(self._ProjectileInfo, self._TargetInfo)
end

---Finish callback
function LogicRandomDropProjectile:OnSpawnProjectileFinished(Result)
    self:FinishWithParams(Result)
end

return LogicRandomDropProjectile
