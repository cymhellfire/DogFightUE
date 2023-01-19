require("UnLua")

local AttributeEnum = require "DogFight.DataBridge.AttributeEnum"

local CardBase = Class()

--========================== Workflow ==========================--

function CardBase:BP_Initialize()
    -- Invoke OnInitialize of any subclass
    if type(self.OnInitialized) == "function" then
        self:OnInitialized()
    end
end

function CardBase:BP_GetCardBasicDesc(OutName, OutDesc)
    -- Invoke SetupCardDesc of any subclass
    if type(self.SetupCardDesc) == "function" then
        local Desc = self:SetupCardDesc()
        OutName = Desc["Name"] or OutName
        OutDesc = Desc["Desc"] or OutDesc
    end

    return OutName, OutDesc
end

--========================== Attribute ==========================--

---Create new attribute to this card based on given arguments.
---根据传入参数为当前卡牌添加新的属性。
---@param InTable table @ Create argument / 构造参数
function CardBase:CreateAttribute(InTable)
    local NameStr = InTable["Name"]
    if NameStr == nil then
        print("[CardBase] Cannot create attribute without name.")
        return
    end

    local DataTypeStr = InTable["DataType"]
    local BaseValue = InTable["Value"] and InTable["Value"] or AttributeEnum.DefaultValue[DataTypeStr]

    -- Set tags
    local TagArray = UE.TArray("")
    local Tags = InTable["Tags"]
    if type(Tags) == "table" and #Tags > 0 then
        for _, v in ipairs(Tags) do
            TagArray:Add(v)
        end
    end

    -- Construct attribute argument
    local NewArgument = UE.FAttributeCreateArgument()
    NewArgument.AttrName = NameStr
    NewArgument.DataType = AttributeEnum.DataType[DataTypeStr]
    NewArgument.Tags = TagArray
    NewArgument["Init" .. DataTypeStr .. "Value"] = BaseValue
    self:AddAttribute(NewArgument)
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
