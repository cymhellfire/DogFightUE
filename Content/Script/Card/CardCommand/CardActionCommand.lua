local CardCommandBase = require "Card.CardCommand.CardCommandBase"

---@class CardActionCommand : CardCommandBase Base class of all card action command.
local CardActionCommand = UnrealClass(CardCommandBase)

function CardActionCommand:OnInit()
    CardCommandBase.OnInit(self)
end

function CardActionCommand:StartCommand()
    CardCommandBase.StartCommand(self)
end

function CardActionCommand:tostring()
    return "CardActionCommand"
end

function CardActionCommand:GetArgumentValue(ArgumentName)
    if self._CardLogic then
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

return CardActionCommand