local BuffTypeDef = require "DogFight.Services.BuffService.BuffTypeDef"

---@class PlayerRoundState : GameFlowStateLogicBase Player can use cards in this state.
local PlayerRoundState = UnrealClass("GameFlow.GameFlowState.GameFlowStateLogicBase")

function PlayerRoundState:OnEnter()
    print("PlayerRoundState: OnEnter")

    -- Record player id
    self.CurPlayerId = UE.UCommonGameFlowFunctionLibrary.GetCurrentPlayerId(self.OwnerState)

    -- Listen to player card events
    GetGameService(self.OwnerState, GameServiceNameDef.LuaEventService):RegisterListener(UE.ELuaEvent.LuaEvent_PlayerCardFinished, self, self.OnCardFinished)

    -- Add use card mapping to current player
    local CurPlayerId = UE.UCommonGameFlowFunctionLibrary.GetCurrentPlayerId(self.OwnerState)
    GetGameService(self.OwnerState, GameServiceNameDef.GameInputService):AddInputMappingByPlayerId(CurPlayerId, UE.EInputMappingType.InputMapping_CardUsing)

    -- Add test modifier to current player character
    ---@type ATopDownStylePlayerController
    local CurPC = UE.UCommonGameplayFunctionLibrary.GetPlayerControllerById(self.OwnerState, CurPlayerId)
    if CurPC then
        ---@type ATopDownStylePlayerCharacter
        local PlayerCharacter = CurPC:GetCharacterPawn()
        if PlayerCharacter then
            -- ---@type BuffService
            -- local BuffService = GetGameService(self.OwnerState, GameServiceNameDef.BuffService)
            -- if BuffService then
            --     ---@type URoundTimerBuff
            --     local NewBuff = BuffService:CreateBuff(BuffTypeDef.AddPhysResist)
            --     NewBuff:SetDuration(1)
            --     NewBuff:Apply(PlayerCharacter)
            -- end

            ---@type URoundTimerBuff
            local NewBuff = NewObject(UE.URoundTimerBuff.StaticClass(), self.OwnerState)
            if NewBuff then
                NewBuff:InitBuff("DogFight.Buff.BuffLogic.AddPhysResistBuff")
                NewBuff:SetDuration(1)
                NewBuff:Apply(PlayerCharacter)
            end
        end
    end
end

function PlayerRoundState:OnExit()
    print("PlayerRoundState: OnExit")

    -- Stop listen to player card events
    GetGameService(self.OwnerState, GameServiceNameDef.LuaEventService):UnregisterListener(UE.ELuaEvent.LuaEvent_PlayerCardFinished, self, self.OnCardFinished)

    -- Remove use card mapping from current player
    local CurPlayerId = UE.UCommonGameFlowFunctionLibrary.GetCurrentPlayerId(self.OwnerState)
    GetGameService(self.OwnerState, GameServiceNameDef.GameInputService):RemoveInputMappingByPlayerId(CurPlayerId, UE.EInputMappingType.InputMapping_CardUsing)
end

function PlayerRoundState:OnCardFinished(InPlayerId, InId)
    -- Skip if the finished card is not used by current player
    if InPlayerId ~= self.CurPlayerId then
        return
    end

    -- Check the remaining card count
    local CardNum = UE.UCommonGameplayFunctionLibrary.GetPlayerCardNums(self.OwnerState, self.CurPlayerId)
    print("PlayerRound: " .. CardNum .. " cards left")
    if CardNum <= 0 then
        self:FinishState()
    end
end

function PlayerRoundState:FinishState()
    -- Construct next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GetGameService(self.OwnerState, GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = "StandardMode.PostPlayerRoundCardState"
        NewArgument.Instigator = Instigator
        self.OwnerState:SetNextState(NewArgument)
    end

    self.OwnerState:Finish()
end

return PlayerRoundState