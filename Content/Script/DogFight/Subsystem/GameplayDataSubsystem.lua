---@class GameplayDataSubsystem : LuaGameInstanceSubsystem
local GameplayDataSubsystem = UnrealClass("DogFight.Subsystem.LuaGameInstanceSubsystem")

---@param InCount number
function GameplayDataSubsystem:SetAIPlayerCount(InCount)
    
end

function GameplayDataSubsystem:StartListenNetworkFailure()
    ---@type LuaEventService
    local LuaEventService = GetGameService(self, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_SessionDismiss, self, self.OnSessionDismiss)
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_NetworkFailure, self, self.OnNetworkFailure)
    end
end

function GameplayDataSubsystem:StopListenNetworkFailure()
    ---@type LuaEventService
    local LuaEventService = GetGameService(self, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_SessionDismiss, self, self.OnSessionDismiss)
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_NetworkFailure, self, self.OnNetworkFailure)
    end
end

---@param self GameplayDataSubsystem
---@param Title string
---@param Msg string
---@param LevelName string
local function AddPendingMessage(self, Title, Msg, LevelName)
    ---@type LuaGameInstance
    local GameInstance = GetGameInstance(self)
    if GameInstance then
        ---@type UPopupMessageSubsystem
        local MessageSubsystem = GameInstance:GetSubsystem("PopupMessageSubsystem")
        if MessageSubsystem then
            local NewMessage = UE.FGenericMessage()
            NewMessage.Title = Title
            NewMessage.Content = Msg
            MessageSubsystem:AddPendingMessage(LevelName, NewMessage)
        end
    end
end

---@param InType ENetworkFailure
function GameplayDataSubsystem:OnNetworkFailure(InType)
    --- Handle connection issues
    if InType == UE.ENetworkFailure.ConnectionLost then
        AddPendingMessage(self, "UI_NetError_Title", "UI_NetError_LostConnection", "MainMenu")

        -- Clear sessions
        ---@type UCommonSessionSubsystem
        local SessionSubsystem = UE.UGameLobbyFunctionLibrary.GetCommonSessionSubSystem(self)
        if SessionSubsystem then
            SessionSubsystem:CleanUpSessions()
        end

        -- Register listener next time create or join a session
        self:StopListenNetworkFailure()
    end
end

---@param Reason EReturnToMainMenuReason
function GameplayDataSubsystem:OnSessionDismiss(Reason)
    ---Clear sessions
    ---@type UCommonSessionSubsystem
    local CommonSessionSubsystem = UE.UGameLobbyFunctionLibrary.GetCommonSessionSubSystem(self)
    if CommonSessionSubsystem then
        CommonSessionSubsystem:CleanUpSessions()
    end

    if Reason == UE.EReturnToMainMenuReason.HostLeft then
        AddPendingMessage(self, "UI_NetError_Title", "UI_NetError_HostLeft", "MainMenu")
    end

    self:StopListenNetworkFailure()
end

return GameplayDataSubsystem