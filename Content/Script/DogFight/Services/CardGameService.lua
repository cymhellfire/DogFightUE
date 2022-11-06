require "Unlua"

local CardGameService = Class("DogFight.Services.GameServiceBase")

function CardGameService:SpawnAndBindCard(Template, ScriptPath, Instigator)
    local NewCard = NewObject(Template, Instigator, nil, ScriptPath)

    return NewCard
end

function CardGameService:DestroyCard(InCard)
    InCard:Destroy()
end

return CardGameService
