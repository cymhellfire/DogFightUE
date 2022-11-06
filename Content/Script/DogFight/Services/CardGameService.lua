require "Unlua"

local CardGameService = Class("DogFight.Services.GameServiceBase")

---Create a card with given script.
---基于给定脚本创建卡牌
---@param Path string @ Path of lua script / 脚本路径
---@param Instigator UObject @ Instigator of creation / 创建发起者
function CardGameService:CreateCard(Path, Instigator)
    -- Get the default card class
    local DefaultClass = self:GetDefaultCardClass()
    if DefaultClass == nil then
        return nil
    end

    -- Create card and bind script
    local NewCard = NewObject(DefaultClass, Instigator, nil, self:ValidateScriptPath(Path))
    return NewCard
end

function CardGameService:GetScriptBasePath()
    return "DogFight.Cards."
end

function CardGameService:DestroyCard(InCard)
    InCard:Destroy()
end

return CardGameService
