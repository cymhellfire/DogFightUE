require "UnLua"

local CardModifierTest = Class("DogFight.CardModifier.CardModifierBase")

function CardModifierTest:GetArgumentTable()
    return {
        ["DataType"] = "Integer",
        ["OperatorType"] = "Multiply",
        ["Value"] = 2,
    }
end

return CardModifierTest
