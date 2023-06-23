local CardTypeDef = require "DogFight.Services.CardService.CardTypeDef"

---@class CardGeneratorService : GameServiceBase Service that hold card generating functionality.
---卡牌生成服务
local CardGeneratorService = UnrealClass("DogFight.Services.GameServiceBase")

function CardGeneratorService:GetConfigPath()
    return "DogFight.Services.CardService.CardConfig"
end

function CardGeneratorService:GetRandomCard()
    local TotalNum = self.Config:GetConfigNum()
    if TotalNum > 0 then
        -- local Index = math.random(0, TotalNum - 1)
        -- local Config = self.Config:GetConfig(Index)
        -- return Config.Id
        return CardTypeDef.AddPhysResist
    end
    
    return nil
end

return CardGeneratorService