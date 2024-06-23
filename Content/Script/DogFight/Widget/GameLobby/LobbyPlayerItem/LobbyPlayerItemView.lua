---@class LobbyPlayerItemView : BP_Widget_LobbyPlayerListItem_C
---@field Data AGameLobbyPlayerState
---@field bHost boolean Whether this player is host
---@field AvatarMiniInfo AvatarMiniInfoView
local LobbyPlayerItemView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local LobbyPlayerItemVM = require("DogFight.Widget.GameLobby.LobbyPlayerItem.LobbyPlayerItemVM")

function LobbyPlayerItemView:PostInitialized()
    local NewVM = InstantiateViewModel(LobbyPlayerItemVM)
    self:BindViewModel(NewVM, {
        {BindKey = "PlayerName",   UIKey = "PlayerName_Text",   DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "PlayerStatus",  UIKey = "PlayerStatus_Text",    DataBinding = DataBinding.TextContextBinding() },
    })
end

---@param self LobbyPlayerItemView
---@param InPlayerState AGameLobbyPlayerState
local function RegisterCallback(self, InPlayerState)
    if InPlayerState then
        InPlayerState.OnReadyStatusChanged:Add(self, self.OnPlayerReadyStateChanged)
        InPlayerState.OnHostStatusChanged:Add(self, self.OnPlayerHostStateChanged)
        InPlayerState.OnPlayerNameChanged:Add(self, self.OnPlayerNameChanged)
        InPlayerState.OnAvatarIdChanged:Add(self, self.OnAvatarIdChanged)
    end
end

---@param self LobbyPlayerItemView
---@param InPlayerState AGameLobbyPlayerState
local function UnregisterCallback(self, InPlayerState)
    if InPlayerState then
        InPlayerState.OnReadyStatusChanged:Remove(self, self.OnPlayerReadyStateChanged)
        InPlayerState.OnHostStatusChanged:Remove(self, self.OnPlayerHostStateChanged)
        InPlayerState.OnPlayerNameChanged:Remove(self, self.OnPlayerNameChanged)
        InPlayerState.OnAvatarIdChanged:Remove(self, self.OnAvatarIdChanged)
    end
end

---@param InPlayerState AGameLobbyPlayerState
local function GetPlayerStatusLocalizeKey(InPlayerState)
    if InPlayerState then
        local bReady = InPlayerState:GetPlayerReadyStatus()
        local bHost = InPlayerState:GetPlayerHostStatus()
        return bHost and "UI_HostPlayer" or (bReady and "UI_Ready" or "UI_PreparingPlayer")
    end
end

---@param self LobbyPlayerItemView
---@param InPlayerState AGameLobbyPlayerState
local function InitDisplay(self, InPlayerState)
    self.bHost = InPlayerState:GetPlayerHostStatus()
    self.ViewModel.PlayerStatus = GetLocalizedString(LocalizationTable.CommonUI, GetPlayerStatusLocalizeKey(InPlayerState))
    
    -- Local player specified logic
    local PlayerId = InPlayerState:GetPlayerId()
    local bIsLocalPlayer = UE.UGameLobbyFunctionLibrary.IsLocalPlayer(self, PlayerId)
    local PlayerInfo = InPlayerState:GetLobbyPlayerInfo()
    local CurAvatarId = PlayerInfo and PlayerInfo.AvatarId or UE.FGameLobbyPlayerAvatarId()
    if bIsLocalPlayer then
        self.AvatarMiniInfo:InitInfo(CurAvatarId, false, function(AvatarId)
            self:ApplyAvatarId(AvatarId)

            -- Record avatar id
            UE.UGameSettingsFunctionLibrary.SetLastAvatarId(AvatarId)
            UE.UGameSettingsFunctionLibrary.SaveGameSettings()
        end)
    else
        self.AvatarMiniInfo:InitInfo(CurAvatarId, true)
    end
end

function LobbyPlayerItemView:OnListItemObjectSet(InObject)
    if self.Data then
        UnregisterCallback(self, self.Data)
    end

    self.Data = InObject:GetData()
    if self.Data then
        self.ViewModel.PlayerName = self.Data:GetPlayerName()
        
        InitDisplay(self, self.Data)
        RegisterCallback(self, self.Data)
    end
end

function LobbyPlayerItemView:OnPlayerReadyStateChanged(InPlayerState, bReady)
    self.ViewModel.PlayerStatus = GetLocalizedString(LocalizationTable.CommonUI, GetPlayerStatusLocalizeKey(InPlayerState))
end

function LobbyPlayerItemView:OnPlayerHostStateChanged(InPlayerState, bHost)
    self.bHost = bHost
    self.ViewModel.PlayerStatus = GetLocalizedString(LocalizationTable.CommonUI, GetPlayerStatusLocalizeKey(InPlayerState))
end

function LobbyPlayerItemView:OnPlayerNameChanged(InPlayerState, InName)
    self.ViewModel.PlayerName = InName
end

---@param InAvatarId FGameLobbyPlayerAvatarId
function LobbyPlayerItemView:ApplyAvatarId(InAvatarId)
    print("LobbyPlayerItemView:ApplyAvatarId")

    ---@type AGameLobbyPlayerController
    local LocalPC = UE.UGameLobbyFunctionLibrary.GetLocalGameLobbyPlayerController(self)
    if LocalPC then
        LocalPC:ServerSelectAvatarId(InAvatarId)
    end
end

---@param InId FGameLobbyPlayerAvatarId
function LobbyPlayerItemView:OnAvatarIdChanged(InPlayerState, InId)
    if self.AvatarMiniInfo then
        self.AvatarMiniInfo:SetAvatarId(InId)
    end
end

return LobbyPlayerItemView