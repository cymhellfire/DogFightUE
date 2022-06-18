require("UnLua")

local UTestCard = Class()

function UTestCard:AcquireCardTargetsImplementation()
    local NewSettings = UE.FTargetAcquireSettings()
    NewSettings.Type = UE.ECardTargetType.CTT_Actor
    NewSettings.TargetCount = 1

    self:AcquireTargetBatch(NewSettings, 0)
end

function UTestCard:CardLogicImplementation()
    local TargetList = self:GetActorTargetListByBatch(0)
    if TargetList:Length() > 0 then
        for i = 1, TargetList:Length() do
            local Target = TargetList:Get(i)
            print("Get target: " .. Target:GetName())
        end
    end
    -- Invoke when card finished
    self:OnCardFinished()
end

return UTestCard
