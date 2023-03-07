local CardCommandBase = require "Card.CardCommand.CardCommandBase"

---@class CardActionCommand : CardCommandBase Base class of all card action command.
local CardActionCommand = Class(CardCommandBase)

function CardActionCommand:StartCommand()
    CardCommandBase.StartCommand(self)
end

function CardActionCommand:tostring()
    return "CardActionCommand"
end

return CardActionCommand