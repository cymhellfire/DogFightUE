require("UnLua")

local UTestCard = Class()

function UTestCard:AcquireCardTargetsImplementation()
    local NewSettings = UE.FTargetAcquireSettings()
    NewSettings.Type = UE.ECardTargetType.CTT_Actor
    NewSettings.TargetCount = 1

    self:AcquireTargetBatch(NewSettings, 0)
end

function UTestCard:CardLogicImplementation()
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

    -- Invoke when card finished
    --self:OnCardFinished()
end

function UTestCard:OnCallbackResult(CommandIndex, Result)
    if CommandIndex == 2 then
        print("Random result: " .. Result)
    end
end

return UTestCard
