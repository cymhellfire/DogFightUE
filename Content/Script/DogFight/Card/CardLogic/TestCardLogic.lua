local CardLogicCommand = require "Card.CardCommand.CardLogicCommand"

---@class TestCardLogic : CardLogicCommand
local TestCardLogic = Class(CardLogicCommand)

local CommandNameDef = {
    AcquireTarget = "AcquireTarget",
    PrintName = "PrintName",
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
    }

    self:RegisterCommandTable(CommandTable)
end

function TestCardLogic:StartCommand()
    CardLogicCommand.StartCommand(self)

    self:RunCommand(CommandNameDef.AcquireTarget)
end

function TestCardLogic:OnAcquireTargetFinished(Result)
    self:RunCommand(CommandNameDef.PrintName)
end

function TestCardLogic:OnPrintNameFinished(Result)
    self:FinishWithParams(Result)
end

return TestCardLogic