require "UnLua"
require "LuaIntegration.Common.UnrealUtils"

---@class InitState Initial state after player enter the game level until all players are loaded.
local InitState = Class("GameFlow.GameFlowState.GameFlowStateLogicBase")

function InitState:OnEnter()
    print("InitState: OnEnter")
    print("InitState: Instigator " .. self.OwnerState.CreateArgument.Instigator:GetName())

    GetGameService(GameServiceNameDef.LuaEventService):RegisterListener(UE.ELuaEvent.LuaEvent_ReadyPlayerCount,
        self, self.OnReadyPlayerCountChanged)
end

function InitState:OnReadyPlayerCountChanged(InCount)
    local GameInstance = GetGameInstance()
    --local AllPlayerCount = GameInstance and GameInstance.GamePlayerCount or 0
    local AllPlayerCount = 2

    print("Ready Player: " .. InCount .. "/" .. AllPlayerCount)

    local NewTitle = GetLocalizedString("ST_InGameUI", "UI_WaitingPlayersTitle", {tostring(InCount), tostring(AllPlayerCount)})
    UE.UInGameMessageFunctionLibrary.SetTitleMessage(NewTitle)

    if tonumber(InCount) >= AllPlayerCount then
        GetGameService(GameServiceNameDef.LuaEventService):UnregisterListener(UE.ELuaEvent.LuaEvent_ReadyPlayerCount,
            self, self.OnReadyPlayerCountChanged)

        -- Add camera movement input to all players
        GetGameService(GameServiceNameDef.GameInputService):MulticastAddInputMapping(UE.EInputMappingType.InputMapping_CameraMove)

        -- Setup next state
        local Instigator = self.OwnerState.CreateArgument.Instigator
        local NewArgument = GetGameService(GameServiceNameDef.GameFlowStateService):GetGameFlowStateCreateArgument(Instigator)
        if NewArgument then
            NewArgument.StateName = "StandardMode.SpawnState"
            NewArgument.Instigator = Instigator
            self.OwnerState:SetNextState(NewArgument)
        end

        self.OwnerState:Finish()
    end
end

return InitState
