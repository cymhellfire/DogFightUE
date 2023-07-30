
local AttributeEnum = {}

---Attribute Data Type
AttributeEnum.DataType = {
    ["None"] = 0,
    ["Boolean"] = 1,
    ["Integer"] = 2,
    ["Float"] = 3,
}

---Modifier Operator Type
AttributeEnum.OperatorType = {
    ["Set"] = 0,
    ["Add"] = 1,
    ["Sub"] = 2,
    ["Multiply"] = 3,
    ["Divide"] = 4,
}

---Default Value of Data Type
local DefaultValue = {
    [AttributeEnum.DataType.Boolean] = false,
    [AttributeEnum.DataType.Integer] = 0,
    [AttributeEnum.DataType.Float] = 0,
}

local DataTypeString = {
    [AttributeEnum.DataType.Boolean] = "Boolean",
    [AttributeEnum.DataType.Integer] = "Integer",
    [AttributeEnum.DataType.Float] = "Float",
}

---Get default value of given data type
---@type AttribtueEnum.DataType
function AttributeEnum:GetDefaultValue(InType)
    return DefaultValue[InType] or 0
end

---Get name string of given data type
---@type AttribtueEnum.DataType
function AttributeEnum:GetDataTypeString(InType)
    return DataTypeString[InType] or "Integer"
end

return AttributeEnum
