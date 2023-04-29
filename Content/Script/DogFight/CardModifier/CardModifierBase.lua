local AttributeEnum = require "DogFight.DataBridge.AttributeEnum"

local CardModifierBase = UnrealClass()

---Construct a FModifierCreateArgument instance based on passed in table.
---基于Lua表构建FModifierCreateArgument实例。
---@param InTable table @ Passed in table / 传入表
---@return FModifierCreateArgument @ Created argument instance / 新建实例
function CardModifierBase:MakeCreateArgumentByTable(InTable)
    local Instance = UE.FModifierCreateArgument()
    local DataTypeStr = InTable["DataType"]
    Instance.DataType = DataTypeStr and AttributeEnum.DataType[DataTypeStr] or AttributeEnum.DataType["None"]
    Instance.OperatorType = InTable["OperatorType"] and AttributeEnum.OperatorType[InTable["OperatorType"]]
        or AttributeEnum.OperatorType["Set"]
    Instance.ApplyRuleString = InTable["ApplyRule"] and InTable["ApplyRule"] or ""

    -- Set init value
    if InTable["Value"] and DataTypeStr then
        Instance["Init" .. DataTypeStr .. "Value"] = InTable["Value"]
    end

    return Instance
end

---Obtain the create argument table for this modifier.
---获取创建修改器使用的参数集
---@return table @ Argument table / 参数集
function CardModifierBase:GetModifierCreateArgument()
    local InitTable = nil
    if self.GetModifierArgumentTable then
        InitTable = self:GetModifierArgumentTable()
    end
    return self:MakeCreateArgumentByTable(InitTable)
end

---Construct a FCardModifierDescArgument instance based on passed in table.
---基于Lua表构建FCardModifierDescArgument实例。
---@param InTable table @ Passed in table / 传入表
---@return FCardModifierDescArgument @ Created argument instance / 新建实例
function CardModifierBase:MakeModifierDescArgumentByTable(InTable)
    local Instance = UE.FCardModifierDescArgument()
    local ModifierName = InTable["Name"] and InTable["Name"] or "Untitled Modifier"
    Instance.ModifierName = ModifierName

    return Instance
end

---Obtain arguments to initialize modifier description object.
---获取初始化修改器描述的参数集
---@return table @ Argument table / 参数集
function CardModifierBase:GetModifierDescArgument()
    local DescTable = nil
    if self.GetDescArgumentTable then
        DescTable = self:GetDescArgumentTable()
    end

    return self:MakeModifierDescArgumentByTable(DescTable)
end

return CardModifierBase
