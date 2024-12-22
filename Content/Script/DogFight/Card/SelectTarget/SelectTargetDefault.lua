local CardLogicCommand = require "Card.CardCommand.CardLogicCommand"
local CardCommandResultDef = require "Card.CardCommand.CardCommandResultDef"

---@class SelectTargetDefault : CardLogicCommand
local SelectTargetDefault = UnrealClass(CardLogicCommand)

local CommandNameDef = {
    AcquireTarget = "AcquireTarget",
}

---Initialize command
function SelectTargetDefault:OnInit(InParam)
    CardLogicCommand.OnInit(self, InParam)

    -- Register all commands
    local CommandTable = {
        [CommandNameDef.AcquireTarget] = {
            Script = "DogFight.Card.CardAction.ActionCardAcquireTarget",
            OnCreate = self.OnAcquireTargetCreated,
            OnFinish = self.OnAcquireTargetFinished,
        }
    }
    self:RegisterCommandTable(CommandTable)
end

function SelectTargetDefault:StartCommand()
    CardLogicCommand.StartCommand(self)

    self:RunCommand(CommandNameDef.AcquireTarget)
end

function SelectTargetDefault:OnAcquireTargetCreated(InCommand)
    if self._TargetInfo then
        InCommand:InitAcquireSettings(self._TargetInfo)
    else
        self:FailedWithParams()
    end
end

function SelectTargetDefault:OnAcquireTargetFinished(Result, TargetInfo)
    if Result == CardCommandResultDef.Succeed then
        self:SuccessWithParams(TargetInfo)
    else
        self:FinishWithParams(Result)
    end
end

return SelectTargetDefault