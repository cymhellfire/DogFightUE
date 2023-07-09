---@class LobbyPlayerItemView : BP_Widget_LobbyPlayerListItem_C
---@field Data AGameLobbyPlayerState
---@field bHost boolean Whether this player is host
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
    end
end

---@param self LobbyPlayerItemView
---@param InPlayerState AGameLobbyPlayerState
local function UnregisterCallback(self, InPlayerState)
    if InPlayerState then
        InPlayerState.OnReadyStatusChanged:Remove(self, self.OnPlayerReadyStateChanged)
        InPlayerState.OnHostStatusChanged:Remove(self, self.OnPlayerHostStateChanged)
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

return LobbyPlayerItemView