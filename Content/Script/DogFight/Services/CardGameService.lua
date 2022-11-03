require "Unlua"

local UCardGameService = Class("DogFight.Services.GameServiceBase")

function UCardGameService:SpawnAndBindCard(Template, ScriptPath, Instigator)
    local NewCard = NewObject(Template, Instigator, nil, ScriptPath)

    return NewCard
end

function UCardGameService:DestroyCard(InCard)
    InCard:Destroy()
end

return UCardGameService
