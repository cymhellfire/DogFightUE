---@class PostPlayerRoundBuffState : GameFlowStateLogicBase Buff accumulation state after player round finished.
local PostPlayerRoundBuffState = UnrealClass("GameFlow.GameFlowState.GameFlowStateLogicBase")

function PostPlayerRoundBuffState:OnEnter()
    local PlayerId = UE.UCommonGameFlowFunctionLibrary.GetCurrentPlayerId(self.OwnerState)
    print("PostPlayerRoundBuffState: OnEnter Player" .. PlayerId)

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
            BuffMgrComp:DoBuffCheck(UE.EBuffCheckType.PostPlayerRound)
            bSuccess = true
        end
    end
    
    -- Enter next state if failed
    if not bSuccess then
        self:EnterNextState()
    end
end

function PostPlayerRoundBuffState:OnDoBuffCheckFinished()
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

function PostPlayerRoundBuffState:EnterNextState()
    -- Construct next state
    local Instigator = self.OwnerState.CreateArgument.Instigator
    local NewArgument = GetGameService(self.OwnerState, GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
    if NewArgument then
        NewArgument.StateName = "StandardMode.GameEndCheckState"
        NewArgument.Instigator = Instigator
        self.OwnerState:SetNextState(NewArgument)
    end

    self.OwnerState:Finish()
end

function PostPlayerRoundBuffState:OnExit()
    print("PostPlayerRoundBuffState: OnExit")
end

return PostPlayerRoundBuffState