local CardTypeDef = require "DogFight.Services.CardService.CardTypeDef"
local CardPool = require "DogFight.Services.CardService.CardPool.DefaultCardPoolCfg"

---@class CardGeneratorService : GameServiceBase Service that hold card generating functionality.
---卡牌生成服务
local CardGeneratorService = UnrealClass("DogFight.Services.GameServiceBase")

function CardGeneratorService:GetConfigPath()
    return "DogFight.Services.CardService.CardConfig"
end

function CardGeneratorService:StartupScript(ServiceName)
    self.Super.StartupScript(self, ServiceName)

    self:LoadCardPoolConfig()
end

function CardGeneratorService:GetRandomCard()
    local TotalNum = self.Config:GetConfigNum()
    if TotalNum > 0 then
        -- local Index = math.random(0, TotalNum - 1)
        -- local Config = self.Config:GetConfig(Index)
        -- return Config.Id
        return CardTypeDef.WeaponAttack
    end
    
    return nil
end

---Load all cards from config file.
function CardGeneratorService:LoadCardPoolConfig()
    self.CardPool = {}
    self.DiscardCardStack = {}

    -- Initialize card pool data
    for Symbol, Sequence in pairs(CardPool.Cards) do
        for Essenstial, NumberList in pairs(Sequence) do
            for _, Number in ipairs(NumberList) do
                local NewCard = {
                    Symbol = Symbol,
                    Number = Number,
                    NumberText = CardPool.NumberToText[Number] or tostring(Number),
                    Essenstial = Essenstial,
                    SymbolSettings = CardPool.SymbolSettings[Symbol],
                }
    
                self.CardPool[#self.CardPool + 1] = NewCard
            end
        end
    end
    print("CardGeneratorService:LoadCardPoolConfig Finished")

    -- Generate discard card stack
    for i = 1, #self.CardPool do
        self.DiscardCardStack[#self.DiscardCardStack + 1] = i
    end
    
    self:RelaunchDiscardCards()
end

---Move all index from discard card stack to available list and randomize the order.
function CardGeneratorService:RelaunchDiscardCards()
    print("CardGeneratorService:RelaunchDiscardCards")

    if #self.DiscardCardStack == 0 then
        error("CardGeneratorService:RelaunchDiscardCards No cards in discard card stack.")
        return
    end

    local Array = UE.TArray(0)
    for _, v in ipairs(self.DiscardCardStack) do
        Array:Add(v)
    end
    self.AvailableCardList = UE.UMathHelperLibrary.RandomizedOrder(Array):ToTable()
    self.DiscardCardStack = {}

    print("CardGeneratorService:RelaunchDiscardCards Finsihed")
end

---Take a card from available list.
---@return number Index of gain card.
function CardGeneratorService:TakeCard()
    if #self.AvailableCardList == 0 then
        print("CardGeneratorService:TakeCard No card avaiable")
        return
    end

    local Index = self.AvailableCardList[#self.AvailableCardList]
    -- Remove taken index from list
    self.AvailableCardList[#self.AvailableCardList] = nil

    print("CardGeneratorService:TakeCard ", Index)

    return Index
end

---Push card into discard card stack.
---@param InIndex number Index of card to discard.
function CardGeneratorService:DiscardCard(InIndex)
    if type(InIndex) ~= "number" then
        return
    end
    print("CardGeneratorService:DiscardCard ", InIndex)

    self.DiscardCardStack[#self.DiscardCardStack + 1] = InIndex
end

---Get card info with given index.
---@param InIndex number Index of card.
---@return table Card info.
function CardGeneratorService:GetCardInfo(InIndex)
    if InIndex > 0 and InIndex <= #self.CardPool then
        return self.CardPool[InIndex]
    end
end

return CardGeneratorService