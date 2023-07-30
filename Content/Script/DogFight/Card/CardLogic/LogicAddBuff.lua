local CardLogicCommand = require "Card.CardCommand.CardLogicCommand"
local BuffTypeDef = require "DogFight.Services.BuffService.BuffTypeDef"

---@class LogicAddBuff : CardLogicCommand Add Physics Resistance Buff to target.
---@field _BuffInfo table Settings of buff to apply.
local LogicAddBuff = UnrealClass(CardLogicCommand)

local CommandNameDef = {
    AcquireTarget = "AcquireTarget",
    ApplyBuff = "ApplyBuff",
}

---Initialize the card workflow
function LogicAddBuff:OnInit(InParams)
    CardLogicCommand.OnInit(self, InParams)

    self._CardInfo = InParams.CardInfo
    -- Record buff information
    self._BuffInfo = InParams.BuffInfo

    -- Register all commands
    local CommandTable = {
        [CommandNameDef.AcquireTarget] = {
            Script = "DogFight.Card.CardAction.ActionCardAcquireTarget",
            OnCreate = self.OnAcquireTargetCreated,
            OnFinish = self.OnAcquireTargetFinished,
        },
        [CommandNameDef.ApplyBuff] = {
            Script = "DogFight.Card.CardAction.ActionAddBuff",
            OnCreate = self.OnApplyBuffCreated,
            OnFinish = self.OnApplyBuffFinished,
        },
    }
    self:RegisterCommandTable(CommandTable)
end

---Start executing
function LogicAddBuff:StartCommand()
    CardLogicCommand.StartCommand(self)

    self:RunCommand(CommandNameDef.AcquireTarget)
end

---@param InCommand ActionCardAcquireTarget
function LogicAddBuff:OnAcquireTargetCreated(InCommand)
    if self._TargetInfo then
        InCommand:InitAcquireSettings(self._TargetInfo)
    else
        self:FailedWithParams()
    end
end

---Command finish callback
function LogicAddBuff:OnAcquireTargetFinished(Result, TargetInfo)
    self._TargetInfo = TargetInfo
    self:RunCommand(CommandNameDef.ApplyBuff)
end

---Command create callback
---@param InCommand ActionAddBuff
function LogicAddBuff:OnApplyBuffCreated(InCommand)
    if self._BuffInfo then
        -- Insert target info
        self._BuffInfo.TargetList = self._TargetInfo
        InCommand:InitBuffSettings(self._BuffInfo)
    else
        self:FailedWithParams()
    end
end

---Finish callback
function LogicAddBuff:OnApplyBuffFinished(Result)
    self:FinishWithParams(Result)
end

return LogicAddBuff
