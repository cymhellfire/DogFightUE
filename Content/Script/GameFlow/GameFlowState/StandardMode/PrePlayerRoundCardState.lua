---@class PrePlayerRoundCardState : GameFlowStateLogicBase Dispatch cards to current player in this state.
local PrePlayerRoundCardState = UnrealClass("GameFlow.GameFlowState.GameFlowStateLogicBase")

function PrePlayerRoundCardState:OnEnter()
    print("PrePlayerRoundCardState: OnEnter")

    -- Dispatch cards to current player
    local CurPlayerId = UE.UCommonGameFlowFunctionLibrary.GetCurrentPlayerId(self.OwnerState)
    if CurPlayerId > 0 then
        self:DispatchCard(CurPlayerId, 2)
    end

    -- Construct next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GetGameService(self.OwnerState, GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = "StandardMode.PlayerRoundState"
        NewArgument.Instigator = Instigator
        self.OwnerState:SetNextState(NewArgument)
    end

    self.OwnerState:Finish()
end

function PrePlayerRoundCardState:OnExit()
    print("PrePlayerRoundCardState: OnExit")
end

---Give cards to specified player.
---@param InPlayerId number Id of player cards give to.
---@param InCount number Total count of cards will dispatch.
function PrePlayerRoundCardState:DispatchCard(InPlayerId, InCount)
    --local TargetPlayerState = UE.UCommonGameplayFunctionLibrary.GetPlayerStateById(InPlayerId)
    local TargetPlayerCtrl = UE.UCommonGameplayFunctionLibrary.GetPlayerControllerById(self.OwnerState, InPlayerId)
    -- Skip if no player controller matches the ID
    if not TargetPlayerCtrl then
        return
    end

    for i = 1, InCount do
        local NewCardLogic = GetGameService(self.OwnerState, GameServiceNameDef.CardGeneratorService):GetRandomCard()
        local NewCard = nil
        -- Create new card by name
        if NewCardLogic then
            NewCard = GetGameService(self.OwnerState, GameServiceNameDef.CardGameService):CreateCard(NewCardLogic, TargetPlayerCtrl)
        end

        -- Dispatch new card to player
        if NewCard then
            UE.UCommonGameplayFunctionLibrary.DispatchCardToPlayer(self.OwnerState, InPlayerId, NewCard)
        end
    end
end

return PrePlayerRoundCardState