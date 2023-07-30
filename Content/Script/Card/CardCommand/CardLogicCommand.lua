local CardCommandBase = require "Card.CardCommand.CardCommandBase"
local CardCommandHelper = require "Card.CardCommand.CardCommandHelper"

---@field _PendingQueue table List of commands will be executed next frame.
---@field _CardInfo table Table of card display infomration.
---@field _AttrInfo table Table of all attributes.
---@field _TargetInfo table Table of all target information.
---@class CardLogicCommand : CardCommandBase Base class of all card logic command.
local CardLogicCommand = UnrealClass(CardCommandBase)

function CardLogicCommand:OnInit(InParam)
    CardCommandBase.OnInit(self)

    if InParam then
        self._AttrInfo = InParam.AttrInfo
        self._CardInfo = InParam.CardInfo
        self._TargetInfo = InParam.TargetInfo
    end

    self._PendingQueue = {}
end

---@param DescObject UCardDescObject
---@param InString string
---@return string
local function ConvertDescArgument(DescObject, InString)
    -- Get integer attribute value
    if string.startWith(InString, "[Int]") then
        local AttrName = string.sub(InString, 6, #InString)
        ---@type UAttributeIntegerWrapperObject
        local AttrWrapper = DescObject:GetIntegerAttributeWrapperByName(AttrName)
        if AttrWrapper then
            return AttrWrapper:GetValue()
        end
    elseif string.startWith(InString, "[Float]") then
        local SuffixString = string.sub(InString, 8, #InString)
        -- Check the output format
        local bPercent = string.startWith(SuffixString, "[Percent]")
        local AttrName = bPercent and string.sub(SuffixString, 10, #SuffixString) or SuffixString
        ---@type UAttributeFloatWrapperObject
        local AttrWrapper = DescObject:GetFloatAttributeWrapperByName(AttrName)
        if AttrWrapper then
            return bPercent and string.format("%.0f%%", AttrWrapper:GetValue() * 100) or AttrWrapper:GetValue()
        end
    end

    -- Use original string
    return InString
end

---@param DescObject UCardDescObject
function CardLogicCommand:SetupDescObject(DescObject)
    local CardInfo = self._CardInfo
    -- Set card name
    if CardInfo and CardInfo.Name then
        local LocalizedName = GetLocalizedString(LocalizationTable.CardDisplay, CardInfo.Name)
        DescObject:SetCardName(LocalizedName)
    end

    -- Add Attributes
    local AttrInfo = self._AttrInfo
    if type(AttrInfo) == "table" and #AttrInfo > 0 then
        ---@type Cardbase
        local Card = self._CardLogic:GetOwnerCard()
        for _, v in ipairs(AttrInfo) do
            local LocalizedAttrName = GetLocalizedString(LocalizationTable.Attribute, "Attr_" .. v.Name)
            Card:CreateAttribute({
                Name = v.Name,
                DisplayName = LocalizedAttrName,
                DataType = v.Type,
                Value = v.Value,
            })
        end
    end
    
    -- Construct card description string
    if CardInfo and CardInfo.Desc then
        local Params = {}
        if type(CardInfo.Desc.Param) == "table" then
            for i = 1, #CardInfo.Desc.Param do
                Params[#Params + 1] = ConvertDescArgument(DescObject, CardInfo.Desc.Param[i])
            end
        end

        DescObject:SetCardDesc(GetLocalizedString(LocalizationTable.CardDisplay, CardInfo.Desc.Key, table.unpack(Params)))
    end
end

---@param DescObject UCardDescObject
function CardLogicCommand:UpdateDescObject(DescObject)
    local CardInfo = self._CardInfo
    -- Set card name
    if CardInfo and CardInfo.Name then
        local LocalizedName = GetLocalizedString(LocalizationTable.CardDisplay, CardInfo.Name)
        DescObject:SetCardName(LocalizedName)
    end

    -- Construct card description string
    if CardInfo and CardInfo.Desc then
        local Params = {}
        if type(CardInfo.Desc.Param) == "table" then
            for i = 1, #CardInfo.Desc.Param do
                Params[#Params + 1] = ConvertDescArgument(DescObject, CardInfo.Desc.Param[i])
            end
        end

        DescObject:SetCardDesc(GetLocalizedString(LocalizationTable.CardDisplay, CardInfo.Desc.Key, table.unpack(Params)))
    end
end

function CardLogicCommand:StartCommand()
    CardCommandBase.StartCommand(self)
end

---Register table of all child card commands.
---@param InTable table Card command config table.
function CardLogicCommand:RegisterCommandTable(InTable)
    self._CommandTable = InTable
end

---Run specified card command.
---@param Name string Command name defined in config table.
---@param bImmediately boolean Whether run the command immediately (starts in next frame as default)
function CardLogicCommand:RunCommand(Name, bImmediately)
    local Config = self._CommandTable[Name]
    if not Config then
        print(self .. ":RunCommand failed. Command config " .. Name .. " not found.")
        return
    end

    local NewCommand = CardCommandHelper.CreateCardCommand(Config.Script)
    if NewCommand then
        -- Set owner
        NewCommand._CardLogic = self._CardLogic
    
        -- Trigger the post create callback
        if Config.OnCreate then
            Config.OnCreate(self, NewCommand)
        end
        -- Register finish callback
        NewCommand:SetCallback(self, Config.OnFinish)

        -- Immediately execute or enqueue
        if bImmediately then
            NewCommand:StartCommand()
        else
            self._PendingQueue[#self._PendingQueue + 1] = NewCommand
        end
    end
end

function CardLogicCommand:TickCommand(DeltaTime)
    CardCommandBase.TickCommand(self, DeltaTime)

    if #self._PendingQueue > 0 then
        -- Execute all commands in pending list
        for _, v in ipairs(self._PendingQueue) do
            v:StartCommand()
        end
        self._PendingQueue = {}
    end
end

function CardCommandBase:tostring()
    return "CardLogicCommand"
end

return CardLogicCommand