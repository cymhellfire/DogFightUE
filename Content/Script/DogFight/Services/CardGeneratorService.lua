require "UnLua"

---@class CardGeneratorService Service that hold card generating functionality.
---卡牌生成服务
local CardGeneratorService = Class("DogFight.Services.GameServiceBase")

function CardGeneratorService:StartupScript(ServiceName)
    self.Super.StartupScript(self, ServiceName)

    -- Temp card list
    self.CardList = {
        "TestCard"
    }
end

function CardGeneratorService:GetRandomCard()
    if self.CardList and #self.CardList > 0 then
        local Index = math.random(1, #self.CardList)
        return self.CardList[Index]
    end
    
    return nil
end

return CardGeneratorService