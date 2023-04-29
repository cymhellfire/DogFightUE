local CardLogicCommand = require "Card.CardCommand.CardLogicCommand"
local BuffTypeDef = require "DogFight.Services.BuffService.BuffTypeDef"

---@class LogicAddPhysResist : CardLogicCommand Add Physics Resistance Buff to target.
local LogicAddPhysResist = UnrealClass(CardLogicCommand)

local CommandNameDef = {
    AcquireTarget = "AcquireTarget",
    ApplyBuff = "ApplyBuff",
}

---@param DescObject UCardDescObject
function LogicAddPhysResist:SetupDescObject(DescObject)
    DescObject:SetCardName("LogicAddPhysResist")
end

---Initialize the card workflow
function LogicAddPhysResist:OnInit()
    CardLogicCommand.OnInit(self)

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
function LogicAddPhysResist:StartCommand()
    CardLogicCommand.StartCommand(self)

    self:RunCommand(CommandNameDef.AcquireTarget)
end

---@param InCommand ActionCardAcquireTarget
function LogicAddPhysResist:OnAcquireTargetCreated(InCommand)
    InCommand:InitAcquireSettings(1, UE.ECardTargetType.CTT_Actor)
end

---Command finish callback
function LogicAddPhysResist:OnAcquireTargetFinished(Result, TargetInfo)
    self._TargetInfo = TargetInfo
    self:RunCommand(CommandNameDef.ApplyBuff)
end

---Command create callback
---@param InCommand ActionAddBuff
function LogicAddPhysResist:OnApplyBuffCreated(InCommand)
    local BuffInfo = {
        BuffId = BuffTypeDef.AddPhysResist,
        Duration = 1,
        TargetList = self._TargetInfo,
    }
    InCommand:InitBuffSettings(BuffInfo)
end

---Finish callback
function LogicAddPhysResist:OnApplyBuffFinished(Result)
    self:FinishWithParams(Result)
end

return LogicAddPhysResist
