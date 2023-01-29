require "UnLua"

---@class PrepareState State that allow players move around to take the position for game process.
local PrepareState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

local function CharacterMoveCountDown(self, Duration)
    -- Add character movement input to all players
    GetGameService(GameServiceNameDef.GameInputService):MulticastAddInputMapping(UE.EInputMappingType.InputMapping_CharacterMove)

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

    -- Remove character movement input to all players
    GetGameService(GameServiceNameDef.GameInputService):MulticastRemoveInputMapping(UE.EInputMappingType.InputMapping_CharacterMove)

    -- Construct next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GetGameService(GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = "StandardMode.InitTimelineState"
        NewArgument.Instigator = Instigator
        self.OwnerState:SetNextState(NewArgument)
    end
    self.OwnerState:Finish()
end

function PrepareState:OnEnter()
    print("PrepareState: OnEnter")

    coroutine.resume(coroutine.create(CharacterMoveCountDown), self, 2)
end

function PrepareState:OnExit()
    print("PrepareState: OnExit")
end

return PrepareState