
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
AttributeEnum.DefaultValue = {
    ["Boolean"] = false,
    ["Integer"] = 0,
    ["Float"] = 0,
}

return AttributeEnum
