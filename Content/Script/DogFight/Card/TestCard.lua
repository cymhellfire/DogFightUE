require("UnLua")

local TestCard = Class("DogFight.Card.CardBase")

function TestCard:OnInitialized()
    -- Create attributes
    self:CreateAttribute({
        Name = "Damage",
        DataType = "Integer",
        Value = 50,
        Tags = {"Tag1", "Tag2"},
    })

    -- Create modifier from service
    local ModifierService = GetGameService(GameServiceNameDef.CardModifierGameService)
    if ModifierService ~= nil then
        local Modifier1 = ModifierService:CreateCardModifier("CardModifierTest", self)
        self:AddModifierObject(Modifier1)
        local Modifier2 = ModifierService:CreateCardModifier("CardModifierTest", self)
        self:AddModifierObject(Modifier2)
        self:RemoveModifierObject(Modifier1)
    end
end

function TestCard:SetupCardDesc()
    return {
        Name = "TestCard",
        Desc = "TestCardDesc",
    }
end

function TestCard:AcquireCardTargetsImplementation()
    local NewSettings = UE.FTargetAcquireSettings()
    NewSettings.Type = UE.ECardTargetType.CTT_Actor
    NewSettings.TargetCount = 1

    self:AcquireTargetBatch(NewSettings, 0)
end

function TestCard:CardLogicImplementation()
    -- local TargetList = self:GetActorTargetListByBatch(0)
    -- if TargetList:Length() > 0 then
    --     for i = 1, TargetList:Length() do
    --         local Target = TargetList:Get(i)
    --         print("Get target: " .. Target:GetName())
    --     end
    -- end
    local WFTParam = UE.FWaitForTimeCardCommandParam()
    WFTParam.Time = 2
    UE.UCardCommandLibrary.WaitForTime(self, WFTParam)

    local PTNParam = UE.FPrintTargetNameCardCommandParam()
    PTNParam.TargetBatch = 0
    UE.UCardCommandLibrary.PrintTargetName(self, PTNParam)

    local WARParam = UE.FWaitAndRandomCallbackCardCommandParam()
    WARParam.Time = 1
    WARParam.RandomRange = UE.FVector2D(1, 4)
    UE.UCardCommandLibrary.WaitAndRandomCallback(self, WARParam)
    self:RegisterCallback(2, self.OnWaitAndRandomCallback)

    -- Invoke when card finished
    --self:OnCardFinished()
end

function TestCard:OnWaitAndRandomCallback(Result)
    print(self:GetName() .. " Random result: " .. Result)

    -- Get attribute value
    local bGot, Damage = self:GetAttributeIntegerValue("Damage")
    if bGot then
        print("Damage = " .. Damage)
    else
        print("Failed to get Damage")
    end
end

return TestCard
