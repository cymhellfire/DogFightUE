local CardCommandBase = require "Card.CardCommand.CardCommandBase"
local CardCommandHelper = require "Card.CardCommand.CardCommandHelper"

---@field _PendingQueue table List of commands will be executed next frame.
---@field _CardInfo table Table of card display infomration.
---@field _AttrInfo table Table of all attributes.
---@class CardLogicCommand : CardCommandBase Base class of all card logic command.
local CardLogicCommand = UnrealClass(CardCommandBase)

function CardLogicCommand:OnInit(InParam)
    CardCommandBase.OnInit(self)

    if InParam then
        self._AttrInfo = InParam.AttrInfo
        self._CardInfo = InParam.CardInfo
    end

    self._PendingQueue = {}
end

function CardLogicCommand:SetupDescObject(DescObject)
    local CardInfo = self._CardInfo
    -- Set card name
    if CardInfo and CardInfo.Name then
        local LocalizedName = GetLocalizedString(self._CardLogic, LocalizationTable.CardDisplay, CardInfo.Name)
        DescObject:SetCardName(LocalizedName)
    end
    -- Construct card description string
    if CardInfo and CardInfo.Desc then
        local Params = UE.TArray("")
        if type(CardInfo.Desc.Param) == "table" then
            for i = 1, #CardInfo.Desc.Param do
                Params:Add(CardInfo.Desc.Param[i])
            end
        end

        local Desc = UE.FCardDescString()
        Desc.DescKey = CardInfo.Desc.Key
        Desc.DescParams = Params
        DescObject:SetCardDesc(Desc)
    end


    -- Add Attributes
    local AttrInfo = self._AttrInfo
    if type(AttrInfo) == "table" and #AttrInfo > 0 then
        ---@type Cardbase
        local Card = self._CardLogic:GetOwnerCard()
        for _, v in ipairs(AttrInfo) do
            Card:CreateAttribute({
                Name = v.Name,
                DataType = v.Type,
                Value = v.Value,
            })
        end
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