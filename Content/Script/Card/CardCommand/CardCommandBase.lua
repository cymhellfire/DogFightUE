local CardCommandResultDef = require "Card.CardCommand.CardCommandResultDef"

---@field _CardLogic CardLogic Owner card logic of this command.
---@field Listener any Object that hold the callback function.
---@field Callback function Callback function that will be triggered when command is finished.
---@class CardCommandBase Base class of all card commands.
local CardCommandBase = UnrealClass()

---Let the command finish and trigger the callback.
---@param self CardCommandBase Command instance.
---@param Result CardCommandResultDef Execution result.
---@param ... any Callback parameters.
local function FinishCommand(self, Result, ...)
    -- Trigger callback
    if self.Listener and self.Callback then
        self.Callback(self.Listener, Result, ...)
    end

    print(self:tostring() .. ":FinishCommand")
end

function CardCommandBase:OnInit()
    print(self:tostring() .. ":OnInit")
end

function CardCommandBase:StartCommand()
    print(self:tostring() .. ":StartCommand")
end

---Set the callback function of this command.
---@param InObj any Object that holding callback function.
---@param InCallback function Callback function to trigger.
function CardCommandBase:SetCallback(InObj, InCallback)
    self.Listener = InObj
    self.Callback = InCallback
end

function CardCommandBase:TickCommand(DeltaTime)

end

function CardCommandBase:SuccessWithParams(...)
    FinishCommand(self, CardCommandResultDef.Succeed, ...)
end

function CardCommandBase:FailedWithParams(...)
    FinishCommand(self, CardCommandResultDef.Failure, ...)
end

function CardCommandBase:CancelWithParams(...)
    FinishCommand(self, CardCommandResultDef.Cancelled, ...)
end

function CardCommandBase:FinishWithParams(Result, ...)
    FinishCommand(self, Result, ...)
end

function CardCommandBase:tostring()
    return "CardCommandBase"
end

return CardCommandBase