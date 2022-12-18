require "UnLua"

local SpawnState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

local function DelayFinish(self, NextStateName)
    UE.UKismetSystemLibrary.Delay(self.OwnerState, 3)

    local bNeedPrepare = self.OwnerState.CreateArgument.bNeedPrepare
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local LoopCount = self.OwnerState.CreateArgument.LoopCount or 0
    local bNeedStop = LoopCount > 6
    if bNeedPrepare then
        local InsertArgument = GameServices.GameFlowStateService:GetGameFlowStateCreateArgument(Instigator)
        InsertArgument.Instigator = Instigator
        InsertArgument.StateName = "StandardMode.PrepareState"
        self.OwnerState:InsertState(InsertArgument)
    end

    if not bNeedStop then
        local NewArgument = GameServices.GameFlowStateService:GetGameFlowStateCreateArgument(Instigator)
        if NewArgument then
            NewArgument.Instigator = Instigator
            NewArgument.StateName = NextStateName
            NewArgument.bNeedPrepare = not bNeedPrepare
            NewArgument.LoopCount = LoopCount + 1
            self.OwnerState:SetNextState(NewArgument)
        end
    end

    self.OwnerState:Finish()
end

local function SpawnPlayerCharacter(self)
    local PCList = UE.UCommonGameFlowFunctionLibrary.GetAllPlayerControllers()
    local Count = PCList:Length()
    if Count > 0 then
        for i = 1, Count do
            local CurPC = PCList:Get(i)
            UE.UCommonGameFlowFunctionLibrary.SpawnPlayerCharacterPawn(CurPC)

            -- Delay 2 second for every player
            UE.UKismetSystemLibrary.Delay(self.OwnerState, 2)
        end
    end
end

function SpawnState:OnEnter()
    print("SpawnState: OnEnter")

    --coroutine.resume(coroutine.create(DelayFinish), self, "StandardMode.SpawnState")
    coroutine.resume(coroutine.create(SpawnPlayerCharacter), self)
end

function SpawnState:OnExit()
    print("SpawnState: OnExit")
end

function SpawnState:OnInterrupted()
    print("SpawnState: OnInterrupted")
end

function SpawnState:OnResume()
    print("SpawnState: OnResume")
end

return SpawnState