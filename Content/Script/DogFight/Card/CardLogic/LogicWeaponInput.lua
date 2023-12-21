local CardLogicCommand = require "Card.CardCommand.CardLogicCommand"

---@class LogicWeaponInput : CardLogicCommand
local LogicWeaponInput = UnrealClass(CardLogicCommand)

local CommandNameDef = {
    AcquireTarget = "AcquireTarget",
    WeaponInput = "WeaponInput",
}

---Initialize the card workflow
function LogicWeaponInput:OnInit(InParam)
    CardLogicCommand.OnInit(self, InParam)

    -- Register all commands
    local CommandTable = {
        [CommandNameDef.AcquireTarget] = {
            Script = "DogFight.Card.CardAction.ActionCardAcquireTarget",
            OnCreate = self.OnAcquireTargetCreated,
            OnFinish = self.OnAcquireTargetFinished,
        },
        [CommandNameDef.WeaponInput] = {
            Script = "DogFight.Card.CardAction.ActionWeaponInput",
            OnCreate = self.OnWeaponInputCreated,
            OnFinish = self.OnWeaponInputFinished,
        },
    }
    self:RegisterCommandTable(CommandTable)
end

---Start executing
function LogicWeaponInput:StartCommand()
    CardLogicCommand.StartCommand(self)

    self:RunCommand(CommandNameDef.AcquireTarget)
end

function LogicWeaponInput:OnAcquireTargetCreated(InCommand)
    if self._TargetInfo then
        InCommand:InitAcquireSettings(self._TargetInfo)
    else
        self:FailedWithParams()
    end
end

---Command finish callback
function LogicWeaponInput:OnAcquireTargetFinished(Result, TargetInfo)
    self._TargetInfo = TargetInfo
    self:RunCommand(CommandNameDef.WeaponInput)
end

---Command create callback
---@param InCommand ActionWeaponInput
function LogicWeaponInput:OnWeaponInputCreated(InCommand)
    InCommand:InitWeaponInputCommand(self._TargetInfo)
end

---Finish callback
function LogicWeaponInput:OnWeaponInputFinished(Result)
    self:FinishWithParams(Result)
end

return LogicWeaponInput
