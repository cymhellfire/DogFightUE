require "UnLua"

local CardModifierTest = Class("DogFight.CardModifier.CardModifierBase")

function CardModifierTest:GetModifierArgumentTable()
    return {
        ["DataType"] = "Integer",
        ["OperatorType"] = "Multiply",
        ["Value"] = 2,
    }
end

function CardModifierTest:GetDescArgumentTable()
    return {
        Name = "TestModifier",
    }
end

return CardModifierTest
