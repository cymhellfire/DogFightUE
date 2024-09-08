---@class SyncCharacterWithPlayerState State that sync current timeline character with owner player.
local SyncCharacterWithPlayerState = UnrealClass("GameFlow.GameFlowState.GameFlowStateLogicBase")

function SyncCharacterWithPlayerState:OnEnter()
    print("SyncCharacterWithPlayerState: OnEnter")

    local CurCharacter = UE.UCommonGameFlowFunctionLibrary.GetCurrentTimelineEntityCharacter(self.OwnerState)
    if CurCharacter:IsValid() then
        UE.UCommonGameplayFunctionLibrary.SetControllingPawnToOwnerPlayer(CurCharacter)
    end

    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GetGameService(self.OwnerState, GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = "StandardMode.PrePlayerRoundBuffState"
        NewArgument.Instigator = Instigator
        self.OwnerState:SetNextState(NewArgument)
    end

    self.OwnerState:Finish()
end

function SyncCharacterWithPlayerState:OnExit()
    print("SyncCharacterWithPlayerState: OnExit")
end

return SyncCharacterWithPlayerState