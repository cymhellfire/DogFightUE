require "UnLua"

---@class CardGeneratorService : GameServiceBase Service that hold card generating functionality.
---卡牌生成服务
local CardGeneratorService = Class("DogFight.Services.GameServiceBase")

function CardGeneratorService:StartupScript(ServiceName)
    self.Super.StartupScript(self, ServiceName)

    -- Temp card list
    self.CardList = {
        "DogFight.Card.CardLogic.LogicCharacterMove",
        "DogFight.Card.CardLogic.TestCardLogic"
    }
end

function CardGeneratorService:GetConfigPath()
    return "DogFight.Services.CardService.CardConfig"
end

function CardGeneratorService:GetRandomCard()
    local TotalNum = self.Config:GetConfigNum()
    if TotalNum > 0 then
        local Index = math.random(0, TotalNum - 1)
        local Config = self.Config:GetConfigById(Index)
        return Config.LogicPath
    end
    
    return nil
end

return CardGeneratorService