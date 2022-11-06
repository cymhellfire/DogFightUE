require "UnLua"

local AttributeEnum = require "DogFight.DataBridge.AttributeEnum"

local CardModifierBase = Class()

CardModifierBase.DefaultBoolean = false
CardModifierBase.DefaultInteger = 0
CardModifierBase.DefaultFloat = 0

---Construct a FModifierCreateArgument instance based on passed in table.
---基于Lua表构建FModifierCreateArgument实例。
---@param InTable table @ Passed in table / 传入表
---@return FModifierCreateArgument @ Created argument instance / 新建实例
function CardModifierBase:MakeCreateArgumentByTable(InTable)
    local Instance = UE.FModifierCreateArgument()
    Instance.DataType = InTable["DataType"] and AttributeEnum.DataType[InTable["DataType"]] or AttributeEnum.DataType["None"]
    Instance.OperatorType = InTable["OperatorType"] and AttributeEnum.OperatorType[InTable["OperatorType"]]
        or AttributeEnum.OperatorType["Set"]
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

---Obtain the create argument table for this modifier.
---获取创建修改器使用的参数集
---@return table @ Argument table / 参数集
function CardModifierBase:GetModifierCreateArgument()
    local InitTable = nil
    if self.GetArgumentTable then
        InitTable = self:GetArgumentTable()
    end
    return self:MakeCreateArgumentByTable(InitTable)
end

return CardModifierBase
