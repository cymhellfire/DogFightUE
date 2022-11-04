require "UnLua"

local UCardModifierTest = Class("DogFight.CardModifiers.CardModifierBase")

function UCardModifierTest:GetModifierCreateArgument()
    local Argument = {
        ["DataType"] = "Integer",
        ["OperatorType"] = "Multiply",
        ["Value"] = 2,
    }

    return self:MakeCreateArgumentByTable(Argument)
end

return UCardModifierTest
