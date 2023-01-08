require "UnLua"

---@class PrepareState State that allow players move around to take the position for game process.
local PrepareState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

local function DelayFinish(self, NextStateName)
    UE.UKismetSystemLibrary.Delay(self.OwnerState, 3)

    -- local Instigator = self.OwnerState.CreateArgument.Instigator
    -- local NewArgument = GameServices.GameFlowStateService:GetGameFlowStateCreateArgument(Instigator)
    -- if NewArgument then
    --     NewArgument.StateName = NextStateName
    --     NewArgument.Instigator = Instigator
    --     self.OwnerState:SetNextState(NewArgument)
    -- end

    self.OwnerState:Finish()
end

local function CharacterMoveCountDown(self, Duration)
    -- Enable character movement
    UE.UCommonGameFlowFunctionLibrary.SetCharacterMoveEnableForAllPlayers(true)

    -- Start count down
    local Countdown = Duration or 5
    for i = Countdown, 1, -1 do
        UE.UInGameMessageFunctionLibrary.SetTitleMessage("Move: " .. i)

        UE.UKismetSystemLibrary.Delay(self.OwnerState, 1)
    end

    -- Disable character movement
    UE.UCommonGameFlowFunctionLibrary.SetCharacterMoveEnableForAllPlayers(false)
end

function PrepareState:OnEnter()
    print("PrepareState: OnEnter")

    --coroutine.resume(coroutine.create(DelayFinish), self, "StandardMode.InitState")
    coroutine.resume(coroutine.create(CharacterMoveCountDown), self, 5)
end

function PrepareState:OnExit()
    print("PrepareState: OnExit")
end

return PrepareState