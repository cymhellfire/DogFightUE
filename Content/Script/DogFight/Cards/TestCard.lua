require("UnLua")

local TestCard = Class("DogFight.Cards.CardBase")

function TestCard:AcquireCardTargetsImplementation()
    -- Create attributes
    self:CreateAttributeInteger("Damage", 50)
    -- Create modifier
    self:CreateModifierForIntegerAttribute("Damage", 2, UE.EModifierOperatorType.MOT_Multiply)
    self:CreateModifierForIntegerAttribute("Damage", 5, UE.EModifierOperatorType.MOT_Addition)
    self:CreateModifierForIntegerAttribute("Damage", 3, UE.EModifierOperatorType.MOT_Divide)

    -- Create modifier with apply rule
    self:CreateModifierForIntegerAttribute("Damage", 5, UE.EModifierOperatorType.MOT_Multiply, "DataType=Float")
    self:CreateModifierForIntegerAttribute("Damage", 10, UE.EModifierOperatorType.MOT_Addition, "[DataType=Float,Name=Damage,Tag(Tag1,Tag2)]")
    self:CreateModifierForIntegerAttribute("Damage", 5, UE.EModifierOperatorType.MOT_Divide, "(Name=Damage,Tag[Tag1,Tag2,Tag3],DataType=Integer)")
    self:CreateModifierForIntegerAttribute("Damage", 2, UE.EModifierOperatorType.MOT_Addition, "(Name=Damage,[DataType=Integer,DataType=Float])")

    -- Create modifier from service
    local ModifierService = _G.GameServices.CardModifierGameService
    if ModifierService ~= nil then
        local Modifier1 = ModifierService:CreateCardModifier("CardModifierTest", self)
        self:AddAttributeModifier(Modifier1)
        local Modifier2 = ModifierService:CreateCardModifier("CardModifierTest", self)
        self:AddAttributeModifier(Modifier2)
        self:RemoveAttributeModifier(Modifier1)
    end

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
