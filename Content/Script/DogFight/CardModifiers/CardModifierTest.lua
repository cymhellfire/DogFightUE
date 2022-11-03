require "UnLua"

local UCardModifierTest = Class()

function UCardModifierTest:GetModifierCreateArgument()
    local NewArgument = UE.FModifierCreateArgument()
    NewArgument.DataType = UE.EAttributeDataType.ADT_Integer
    NewArgument.OperatorType = UE.EModifierOperatorType.MOT_Multiply

    return NewArgument
end

function UCardModifierTest:GetInitialIntegerValue()
    return 2
end

return UCardModifierTest
