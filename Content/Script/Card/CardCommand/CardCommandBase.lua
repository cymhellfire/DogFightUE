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

---Convert argument list with GetArgumentValue function.
---@param InTable table Table of arguments to convert.
---@return table Table of converted argument value.
function CardCommandBase:ConvertArgumentTable(InTable)
    if type(InTable) ~= "table" then
        return
    end

    local NewArgs = {}
    for k, v in pairs(InTable) do
        NewArgs[k] = self:GetArgumentValue(v)
    end

    return NewArgs
end

---Get the actual value of given argument.
---eg. '[Int]Damage' will be converted to the actual integer value of attribute with name 'Damage'.
---Argument without data type prefix will remains origin value.
function CardCommandBase:GetArgumentValue(ArgumentName)
    if self._CardLogic then
        if type(ArgumentName) == "string" then
            -- Find data type string
            local Start, End, TypePrefix = string.find(ArgumentName, "(%[.*%])")
            if Start and End then
                local Name = string.sub(ArgumentName, End + 1, #ArgumentName)
                if TypePrefix then
                    local bFound, Value
                    local Card = self._CardLogic:GetOwnerCard()
                    if Card then
                        if TypePrefix == "[Int]" then
                            bFound, Value = Card:GetAttributeIntegerValue(Name)
                        elseif TypePrefix == "[Float]" then
                            bFound, Value = Card:GetAttributeFloatValue(Name)
                        end
                    end

                    return Value
                end
            end
        end
    end

    return ArgumentName
end

function CardCommandBase:tostring()
    return "CardCommandBase"
end

return CardCommandBase