local CardLogicCommand = require "Card.CardCommand.CardLogicCommand"
local ProjectileNameDef = require "DogFight.Services.ProjectileService.ProjectileNameDef"

---@class TestCardLogic : CardLogicCommand
local TestCardLogic = Class(CardLogicCommand)

local CommandNameDef = {
    AcquireTarget = "AcquireTarget",
    PrintName = "PrintName",
    FireProjectile = "FireProjectile",
}

function TestCardLogic:OnInit()
    CardLogicCommand.OnInit(self)

    local CommandTable = {
        [CommandNameDef.AcquireTarget] = {
            Script = "DogFight.Card.CardAction.ActionCardAcquireTarget",
            OnFinish = self.OnAcquireTargetFinished,
        },
        [CommandNameDef.PrintName] = {
            Script = "DogFight.Card.CardAction.ActionPrintTargetName",
            OnFinish = self.OnPrintNameFinished,
        },
        [CommandNameDef.FireProjectile] = {
            Script = "DogFight.Card.CardAction.ActionLaunchProjectile",
            OnCreate = self.OnFireProjectileCreated,
            OnFinish = self.OnFireProjectileFinished,
        },
    }

    self:RegisterCommandTable(CommandTable)
end

function TestCardLogic:StartCommand()
    CardLogicCommand.StartCommand(self)

    self:RunCommand(CommandNameDef.AcquireTarget)
end

function TestCardLogic:OnAcquireTargetFinished(Result, TargetInfo)
    self._TargetInfo = TargetInfo
    self:RunCommand(CommandNameDef.PrintName)
end

function TestCardLogic:OnPrintNameFinished(Result)
    self:RunCommand(CommandNameDef.FireProjectile)
end

---@param InCommand ActionLaunchProjectile
function TestCardLogic:OnFireProjectileCreated(InCommand)
    local ProjectileInfo = {
        Name = ProjectileNameDef.HomingProjectile,
        MuzzleSpeed = 500,
    }
    InCommand:SetCommandInfo(ProjectileInfo, self._TargetInfo)
end

function TestCardLogic:OnFireProjectileFinished(Result)
    self:FinishWithParams(Result)
end

return TestCardLogic