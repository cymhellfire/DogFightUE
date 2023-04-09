require "Unlua"
require "LuaIntegration.Common.UnrealUtils"

---@class CardGameService Service to handle card relative functionality.
local CardGameService = Class("DogFight.Services.GameServiceBase")

function CardGameService:StartupScript(ServiceName)
    self.Super.StartupScript(self, ServiceName)

    ---@type number Available card instance Id.
    self.CardInstanceId = 0
end

---Create a card with given logic script path.
---@param LogicPath string Path of card logic script
---@param Instigator AController Instigator of creation
function CardGameService:CreateCard(LogicPath, Instigator)
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
    NewCard:SetCardLogicPath(LogicPath)
    -- Set instance id to new card
    NewCard:SetInstanceId(self:GetNewCardInstanceId())
    -- Set owner controller
    NewCard:SetOwnerController(Instigator)
    return NewCard
end

function CardGameService:GetScriptBasePath()
    return "DogFight.Card."
end

function CardGameService:DestroyCard(InCard)
    InCard:Destroy()
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
