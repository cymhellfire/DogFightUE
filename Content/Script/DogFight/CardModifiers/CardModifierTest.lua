require "UnLua"

local CardModifierTest = Class("DogFight.CardModifiers.CardModifierBase")

function CardModifierTest:GetArgumentTable()
    return {
        ["DataType"] = "Integer",
        ["OperatorType"] = "Multiply",
        ["Value"] = 2,
    }
end

return CardModifierTest
