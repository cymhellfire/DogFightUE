require "UnLua"

---@class SpawnState State that spawn character for all players.
local SpawnState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

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

    -- Setup next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GetGameService(GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.Instigator = Instigator
        NewArgument.StateName = "StandardMode.PrepareState"
        self.OwnerState:SetNextState(NewArgument)
    end

    self.OwnerState:Finish()
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