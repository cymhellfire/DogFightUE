require "UnLua"
require "Common.StringHelper"

local CardModifierGameService = Class("DogFight.Services.GameServiceBase")

---Create a card modifier with given script.
---基于给定脚本创建卡牌修改器
---@param Path string @ Path of lua script / 脚本路径
---@param Instigator UObject @ Instigator of creation / 创建发起者
function CardModifierGameService:CreateCardModifier(Path, Instigator)
    -- Get the default modifier class
    local DefaultClass = self:GetDefaultModifierClass()
    if DefaultClass == nil then
        return nil
    end

    -- Create modifier and bind script
    local NewModifier = NewObject(DefaultClass, Instigator, nil, self:ValidateScriptPath(Path))
    return NewModifier
end

function CardModifierGameService:GetScriptBasePath()
    return "DogFight.CardModifiers."
end

return CardModifierGameService
