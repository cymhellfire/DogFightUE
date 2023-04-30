local TestCard = UnrealClass("DogFight.Card.CardBase")

function TestCard:OnInitialized()
    -- Create attributes
    self:CreateAttribute({
        Name = "Damage",
        DataType = "Integer",
        Value = 50,
        Tags = {"Tag1", "Tag2"},
    })

    -- Create modifier from service
    local ModifierService = GetGameService(self, GameServiceNameDef.CardModifierGameService)
    if ModifierService ~= nil then
        local Modifier1 = ModifierService:CreateCardModifier("CardModifierTest", self)
        self:AddModifierObject(Modifier1)
        local Modifier2 = ModifierService:CreateCardModifier("CardModifierTest", self)
        self:AddModifierObject(Modifier2)
        self:RemoveModifierObject(Modifier1)
    end

    -- Set card logic path
    --self.LogicScriptPath = "DogFight.Card.CardLogic.TestCardLogic"
    self.LogicScriptPath = "DogFight.Card.CardLogic.LogicCharacterMove"
end

function TestCard:SetupCardDesc()
    return {
        Name = "TestCard",
        Desc = "TestCardDesc",
    }
end

return TestCard
