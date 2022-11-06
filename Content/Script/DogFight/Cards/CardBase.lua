require("UnLua")

local AttributeEnum = require "DogFight.DataBridge.AttributeEnum"

local CardBase = Class()

--========================== Workflow ==========================--

function CardBase:BP_Initialize()
    -- Construct attribute creating function table
    self.AttributeCreateHandle = {
        ["Boolean"] = self.CreateAttributeBool,
        ["Integer"] = self.CreateAttributeInteger,
        ["Float"] = self.CreateAttributeFloat,
    }

    -- Invoke OnInitialize of any subclass
    if type(self.OnInitialized) == "function" then
        self:OnInitialized()
    end
end

--========================== Attribute ==========================--

---Add new attribute to this card based on given arguments.
---根据传入参数为当前卡牌添加新的属性。
---@param InTable table @ Create argument / 构造参数
function CardBase:AddAttribute(InTable)
    local NameStr = InTable["Name"]
    if NameStr == nil then
        print("[CardBase] Cannot create attribute without name.")
        return
    end

    local DataTypeStr = InTable["DataType"]
    local BaseValue = InTable["Value"] and InTable["Value"] or AttributeEnum.DefaultValue[DataTypeStr]

    -- Invoke creation handler
    if self.AttributeCreateHandle[DataTypeStr] then
        self.AttributeCreateHandle[DataTypeStr](self, NameStr, BaseValue)
    else
        print("[CardBase] Unknown data type: " .. DataTypeStr)
        return
    end

    -- Set tags
    local Tags = InTable["Tags"]
    if type(Tags) ~= "table" or #Tags == 0 then
        return
    end

    local TagArray = UE.TArray("")
    for _, v in ipairs(Tags) do
        TagArray:Add(v)
    end
    self:SetAttributeTags(NameStr, TagArray)
end

--====================== Concurrent Command =======================

function CardBase:RegisterCallback(CommandIndex, Callback)
    if self.CallbackMap == nil then
        self.CallbackMap = {}
    end

    self.CallbackMap[CommandIndex] = Callback
end

function CardBase:OnCallbackResult(CommandIndex, Result)
    if self.CallbackMap == nil or self.CallbackMap[CommandIndex] == nil then
        return
    end

    self.CallbackMap[CommandIndex](self, Result)
end

return CardBase
