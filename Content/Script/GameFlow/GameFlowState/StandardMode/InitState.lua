require "LuaIntegration.Common.UnrealUtils"

---@class InitState : GameFlowStateLogicBase Initial state after player enter the game level until all players are loaded.
local InitState = UnrealClass("GameFlow.GameFlowState.GameFlowStateLogicBase")

function InitState:OnEnter()
    print("InitState: OnEnter")
    print("InitState: Instigator " .. self.OwnerState.CreateArgument.Instigator:GetName())

    GetGameService(self.OwnerState, GameServiceNameDef.LuaEventService):RegisterListener(UE.ELuaEvent.LuaEvent_ReadyPlayerCount,
        self, self.OnReadyPlayerCountChanged)

    -- self.VfxCount = 1
    -- ---@type TimerService
    -- local TimerService = GetGameService(self.OwnerState, GameServiceNameDef.TimerService)
    -- if TimerService then
    --     self.VfxTimer = TimerService:RegisterTimer(self, self.LoopVfxTimerExpired, 2, true)
    -- end
end

function InitState:OnReadyPlayerCountChanged(InCount)
    local GameInstance = GetGameInstance(self.OwnerState)
    --local AllPlayerCount = GameInstance and GameInstance.GamePlayerCount or 0
    local AllPlayerCount = 2

    print("Ready Player: " .. InCount .. "/" .. AllPlayerCount)

    local NewTitle = GetLocalizedString("ST_InGameUI", "UI_WaitingPlayersTitle", InCount, AllPlayerCount)
    UE.UInGameMessageFunctionLibrary.SetTitleMessage(self.OwnerState, NewTitle)

    if tonumber(InCount) >= AllPlayerCount then
        GetGameService(self.OwnerState, GameServiceNameDef.LuaEventService):UnregisterListener(UE.ELuaEvent.LuaEvent_ReadyPlayerCount,
            self, self.OnReadyPlayerCountChanged)

        ---@type GameInputService
        local InputService = GetGameService(self.OwnerState, GameServiceNameDef.GameInputService)
        if InputService then
            InputService:MulticastAddInputMapping(UE.EInputMappingType.InputMapping_InGameCommon, UE.EInputMappingPriority.Lowest)
            -- Add camera movement input to all players
            InputService:MulticastAddInputMapping(UE.EInputMappingType.InputMapping_CameraMove)
        end

        -- Setup next state
        local Instigator = self.OwnerState.CreateArgument.Instigator
        local NewArgument = GetGameService(self.OwnerState, GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
        if NewArgument then
            NewArgument.StateName = "StandardMode.SpawnState"
            NewArgument.Instigator = Instigator
            self.OwnerState:SetNextState(NewArgument)
        end

        self.OwnerState:Finish()
    end
end

function InitState:LoopVfxTimerExpired()
    local PosX = math.random(-400, 400)
    local PosY = math.random(-400, 400)

    local EffectId = (self.VfxCount % 5 == 0) and 1 or 0

    UE.UCommonGameplayFunctionLibrary.SpawnGameEffectAtPos(self.OwnerState, EffectId, UE.FVector(PosX, PosY, 0), UE.FRotator(0, 0, 0))
    self.VfxCount = self.VfxCount + 1;

    -- Unregister after 5
    if self.VfxCount > 5 then
        ---@type TimerService
        local TimerService = GetGameService(self.OwnerState, GameServiceNameDef.TimerService)
        if TimerService then
            TimerService:UnregisterTimer(self.VfxTimer)
            self.VfxTimer = nil
        end
    end
end

return InitState
