require "LuaIntegration.Common.UnrealUtils"
local json = require "Common.json"

---@class CardGameService : UCardGameService Service to handle card relative functionality.
local CardGameService = UnrealClass("DogFight.Services.GameServiceBase")

function CardGameService:StartupScript(ServiceName)
    self.Super.StartupScript(self, ServiceName)

    ---@type number Available card instance Id.
    self.CardInstanceId = 0
end

---Create a card with given config id.
---@param CardConfigId number Id of card config
---@param Instigator AController Instigator of creation
function CardGameService:CreateCard(CardConfigId, Instigator)
    -- Ensure the type of Instigator
    local OwnerCtrl = IsDerivedFrom(Instigator, UE.AController.StaticClass())
    if not OwnerCtrl then
        print("CardGameService: Invalid instigator for create card.")
    end

    -- Get the default card class
    local DefaultClass = self:GetDefaultCardClass()
    if DefaultClass == nil then
        return nil
    end

    ---@type CardBase Create card object
    local NewCard = NewObject(DefaultClass, Instigator)
    -- Set card logic 
    NewCard:SetCardLogicId(CardConfigId)
    -- Set instance id to new card
    NewCard:SetInstanceId(self:GetNewCardInstanceId())
    -- Set owner controller
    NewCard:SetOwnerController(Instigator)

    -- Set card info
    ---@type CardGeneratorService
    local CardGeneratorService = GetGameService(self, GameServiceNameDef.CardGeneratorService)
    if CardGeneratorService then
        local PokerIndex = CardGeneratorService:TakeCard()
        if PokerIndex then
            local CardInfo = CardGeneratorService:GetCardInfo(PokerIndex)
            if CardInfo then
                local ExtraInfoTable = table.deepCopy(CardInfo)
                ExtraInfoTable.CardIndex = PokerIndex

                local InfoString = json.encode(ExtraInfoTable)
                NewCard:SetCardExtraInfo(InfoString)
            end
        end
    end
    return NewCard
end

function CardGameService:GetScriptBasePath()
    return "DogFight.Card."
end

function CardGameService:DestroyCard(InCard)
    InCard:Destroy()

    local ExtraInfoString = InCard:GetCardExtraInfo()
    if ExtraInfoString and #ExtraInfoString > 0 then
        local ExtraInfoTable = json.decode(ExtraInfoString)
        local PokerIndex = ExtraInfoTable and ExtraInfoTable.CardIndex
        if PokerIndex then
            ---@type CardGeneratorService
            local CardGeneratorService = GetGameService(self, GameServiceNameDef.CardGeneratorService)
            if CardGeneratorService then
                CardGeneratorService:DiscardCard(PokerIndex)
            end
        end
    end
end

---Get a unused instance Id.
---@return number Unused instance id.
function CardGameService:GetNewCardInstanceId()
    local Result = self.CardInstanceId
    -- Increase the instance Id
    self.CardInstanceId = self.CardInstanceId + 1
    return Result;
end

return CardGameService
