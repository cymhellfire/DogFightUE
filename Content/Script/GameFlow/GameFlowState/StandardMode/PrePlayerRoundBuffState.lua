---@class PrePlayerRoundBuffState : GameFlowStateLogicBase Buff accumulation state before player round begin.
---@field _DelegateHelper DelegateHelper Delegate handle instance for do buff check callback.
local PrePlayerRoundBuffState = UnrealClass("GameFlow.GameFlowState.GameFlowStateLogicBase")

function PrePlayerRoundBuffState:OnEnter()
    local PlayerId = UE.UCommonGameFlowFunctionLibrary.GetCurrentPlayerId(self.OwnerState)
    print("PrePlayerRoundBuffState: OnEnter Player" .. PlayerId)

    local bSuccess = false
    -- Get buff queue to check
    local CurCharacter = UE.UCommonGameplayFunctionLibrary.GetPlayerCharacterById(self.OwnerState, PlayerId)
    if CurCharacter then
        ---@type UBuffManagerComponent
        local BuffMgrComp = CurCharacter.BuffManagerComponent

        ---@type DelegateHelperService
        local DelegateHelperService = GetGameService(self.OwnerState, GameServiceNameDef.DelegateHelperService)
        if DelegateHelperService then
            self._DelegateHelper = DelegateHelperService:BindCallback(BuffMgrComp.OnDoCheckFinished, self, self.OnDoBuffCheckFinished)

            -- Start buff check
            BuffMgrComp:DoBuffCheck(UE.EBuffCheckType.PrePlayerRound)
            bSuccess = true
        end
    end
    
    -- Enter next state if failed
    if not bSuccess then
        self:EnterNextState()
    end
end

function PrePlayerRoundBuffState:OnDoBuffCheckFinished()
    -- Unregister callback
    if self._DelegateHelper then
        ---@type DelegateHelperService
        local DelegateHelperService = GetGameService(self.OwnerState, GameServiceNameDef.DelegateHelperService)
        if DelegateHelperService then
            DelegateHelperService:ReleaseDelegateHelper(self._DelegateHelper)
            self._DelegateHelper = nil
        end
    end

    self:EnterNextState()
end

function PrePlayerRoundBuffState:EnterNextState()
    -- Construct next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GetGameService(self.OwnerState, GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = "StandardMode.PrePlayerRoundCardState"
        NewArgument.Instigator = Instigator
        self.OwnerState:SetNextState(NewArgument)
    end

    self.OwnerState:Finish()
end

function PrePlayerRoundBuffState:OnExit()
    print("PrePlayerRoundBuffState: OnExit")
end

return PrePlayerRoundBuffState