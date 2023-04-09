local CardLogicCommand = require "Card.CardCommand.CardLogicCommand"
local CardCommandResultDef = require "Card.CardCommand.CardCommandResultDef"

---@class LogicCharacterMove : CardLogicCommand
local LogicCharacterMove = Class(CardLogicCommand)

local CommandNameDef = {
    AcquireTarget = "AcquireTarget",
    MoveToPosition = "MoveToPosition",
}

---Initialize the card workflow
function LogicCharacterMove:OnInit()
    CardLogicCommand.OnInit(self)

    -- Register all commands
    local CommandTable = {
        [CommandNameDef.AcquireTarget] = {
            Script = "DogFight.Card.CardAction.ActionCardAcquireTarget",
            OnCreate = self.OnAcquireTargetCreated,
            OnFinish = self.OnAcquireTargetFinished,
        },
        [CommandNameDef.MoveToPosition] = {
            Script = "DogFight.Card.CardAction.ActionMoveToPosition",
            OnCreate = self.OnMoveToPositionCreated,
            OnFinish = self.OnMoveToPositionFinished,
        },
    }
    self:RegisterCommandTable(CommandTable)
end

---Start executing
function LogicCharacterMove:StartCommand()
    CardLogicCommand.StartCommand(self)

    self:RunCommand(CommandNameDef.AcquireTarget)
end

---@param InCommand ActionCardAcquireTarget
function LogicCharacterMove:OnAcquireTargetCreated(InCommand)
    InCommand:InitAcquireSettings(1, UE.ECardTargetType.CTT_Point)
end

---Command finish callback
function LogicCharacterMove:OnAcquireTargetFinished(Result, TargetInfo)
    if Result ~= CardCommandResultDef.Succeed then
        self:FinishWithParams(Result)
        return
    end

    self._TargetInfo = TargetInfo
    self:RunCommand(CommandNameDef.MoveToPosition)
end

---Command create callback
---@param InCommand ActionMoveToPosition
function LogicCharacterMove:OnMoveToPositionCreated(InCommand)
    InCommand:InitMoveCommand(self._TargetInfo)
end

---Finish callback
function LogicCharacterMove:OnMoveToPositionFinished(Result)
    self:FinishWithParams(Result)
end

return LogicCharacterMove
