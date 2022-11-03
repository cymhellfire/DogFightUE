require "UnLua"

local UCardModifierGameService = Class("DogFight.Services.GameServiceBase")

function UCardModifierGameService:CreateCardModifier(Path, Instigator)
    -- Get the default modifier class
    local DefaultClass = self:GetDefaultModifierClass()
    if DefaultClass == nil then
        return nil
    end

    local NewModifier = NewObject(DefaultClass, Instigator, nil, Path)
    -- Initialize new modifier instance
    NewModifier:Initialize()
    return NewModifier
end

return UCardModifierGameService
