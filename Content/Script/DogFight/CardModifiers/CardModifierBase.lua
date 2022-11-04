require "UnLua"

local CardModifierBase = Class()

CardModifierBase.DataType = {
    ["None"] = 0,
    ["Boolean"] = 1,
    ["Integer"] = 2,
    ["Float"] = 3,
}

CardModifierBase.OperatorType = {
    ["Set"] = 0,
    ["Add"] = 1,
    ["Sub"] = 2,
    ["Multiply"] = 3,
    ["Divide"] = 4,
}

CardModifierBase.DefaultBoolean = false
CardModifierBase.DefaultInteger = 0
CardModifierBase.DefaultFloat = 0

---Construct a FModifierCreateArgument instance based on passed in table.
---基于Lua表构建FModifierCreateArgument实例。
---@param InTable table @ Passed in table / 传入表
---@return FModifierCreateArgument @ Created argument instance / 新建实例
function CardModifierBase:MakeCreateArgumentByTable(InTable)
    local Instance = UE.FModifierCreateArgument()
    Instance.DataType = InTable["DataType"] and self.DataType[InTable["DataType"]] or self.DataType["None"]
    Instance.OperatorType = InTable["OperatorType"] and self.OperatorType[InTable["OperatorType"]]
        or self.OperatorType["Set"]
    Instance.ApplyRuleString = InTable["ApplyRule"] and InTable["ApplyRule"] or ""

    -- Record default value based on type
    local InValue = InTable["Value"]
    local ValueType = type(InValue)
    if ValueType == "number" then
        self.DefaultInteger = InValue
        self.DefaultFloat = InValue
    elseif ValueType == "boolean" then
        self.DefaultBoolean = InValue
    end

    return Instance
end

function CardModifierBase:GetInitialBooleanValue()
    return self.DefaultBoolean
end

function CardModifierBase:GetInitialIntegerValue()
    return self.DefaultInteger
end

function CardModifierBase:GetInitialFloatValue()
    return self.DefaultFloat
end

return CardModifierBase
