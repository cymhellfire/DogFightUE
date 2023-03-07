local CardActionCommand = require "Card.CardCommand.CardActionCommand"

---@class ActionPrintTargetName : CardActionCommand
local ActionPrintTargetName = Class(CardActionCommand)

function ActionPrintTargetName:StartCommand()
    CardActionCommand.StartCommand(self)

    print(self:tostring() .. ": target name is null")

    self:SuccessWithParams()
end

function ActionPrintTargetName:tostring()
    return "ActionPrintTargetName"
end

return ActionPrintTargetName