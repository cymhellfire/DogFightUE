local CardLogicCommand = require "Card.CardCommand.CardLogicCommand"
local ProjectileTypeDef = require "DogFight.Services.ProjectileService.ProjectileTypeDef"

---@class LogicFireball : CardLogicCommand Shoot fireball toward target.
local LogicFireball = UnrealClass(CardLogicCommand)

local CommandNameDef = {
    AcquireTarget = "AcquireTarget",
    Fireball = "Fireball",
}

---@param DescObject UCardDescObject
function LogicFireball:SetupDescObject(DescObject)
    DescObject:SetCardName("LogicFireball")
end

---Initialize the card workflow
function LogicFireball:OnInit()
    CardLogicCommand.OnInit(self)

    -- Register all commands
    local CommandTable = {
        [CommandNameDef.AcquireTarget] = {
            Script = "DogFight.Card.CardAction.ActionCardAcquireTarget",
            OnFinish = self.OnAcquireTargetFinished,
        },
        [CommandNameDef.Fireball] = {
            Script = "DogFight.Card.CardAction.ActionLaunchProjectile",
            OnCreate = self.OnFireProjectileCreated,
            OnFinish = self.OnFireProjectileFinished,
        },
    }
    self:RegisterCommandTable(CommandTable)
end

---Start executing
function LogicFireball:StartCommand()
    CardLogicCommand.StartCommand(self)

    self:RunCommand(CommandNameDef.AcquireTarget)
end

---Command finish callback
function LogicFireball:OnAcquireTargetFinished(Result, TargetInfo)
    self._TargetInfo = TargetInfo
    self:RunCommand(CommandNameDef.Fireball)
end

---Command create callback
function LogicFireball:OnFireProjectileCreated(InCommand)
    local ProjectileInfo = {
        Id = ProjectileTypeDef.Fireball,
        MuzzleSpeed = 1500,
    }
    InCommand:SetCommandInfo(ProjectileInfo, self._TargetInfo)
end

---Finish callback
function LogicFireball:OnFireProjectileFinished(Result)
    self:FinishWithParams(Result)
end

return LogicFireball
