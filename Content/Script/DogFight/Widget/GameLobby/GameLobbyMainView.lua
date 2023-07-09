---@class GameLobbyMainView : BP_Widget_GameLobby_C
---@field LocalPlayerState AGameLobbyPlayerState Local player state this UI listen to.
---@field bIsServer boolean Whether current client is hosting server.
---@field SelectedGameExperience UGameplayExperience Gameplay experience currently selected.
---@field PendingGameExperience UGameplayExperience Gameplay experience that not confirm to select.
local GameLobbyMainView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local GameLobbyMainVM = require("DogFight.Widget.GameLobby.GameLobbyMainVM")
local ListWrapper = require("Common.ListView.ListViewWrapper")

local GameWidgetNameDef = require("DogFight.Services.GameWidgetService.GameWidgetNameDef")

---@param self GameLobbyMainView
local function RegisterCallback(self)
    self.LocalPlayerState = UE.UGameLobbyFunctionLibrary.GetLocalGameLobbyPlayerState(self)
    if self.LocalPlayerState then
        self.LocalPlayerState.OnReadyStatusChanged:Add(self, self.OnLocalPlayerReadyStatusChanged)
    end
end

---@param self GameLobbyMainView
local function UnRegisterCallback(self)
    if self.LocalPlayerState then
        self.LocalPlayerState.OnReadyStatusChanged:Remove(self, self.OnLocalPlayerReadyStatusChanged)
    end
end

---@param self GameLobbyMainView
---@param InExperience UGameplayExperience Gameplay experience init display with.
local function InitMapInfo(self, InExperience)
    if InExperience then
        self.ViewModel.MapNameLabel = InExperience.Title
        self.ViewModel.MapPreviewImage = InExperience.MapPreview
    elseif self.SelectedGameExperience then
        self.ViewModel.MapNameLabel = self.SelectedGameExperience.Title
        self.ViewModel.MapPreviewImage = self.SelectedGameExperience.MapPreview
    end
end

---@param self GameLobbyMainView
local function LoadGameExperiences(self)
    local GameplayDataSubsystem = UE.UCommonGameplayFunctionLibrary.GetGameplayDataSubsystem(self)
    if GameplayDataSubsystem then
        local ExperienceList = GameplayDataSubsystem:GetGameplayExperiencesByType(UE.EGameplayExperienceType.Skirmish):ToTable()
        if #ExperienceList > 0 then
            self.MapList:LoadDataByList(ExperienceList)

            -- Select first experience as default
            self.PendingGameExperience = ExperienceList[1]
            self:OnConfirmMapButtonClicked()
        else
            self.MapList:Clear()
        end
    end
end

---@param self GameLobbyMainView
local function DisplayCurrentExperience(self)
    ---@type AGameLobbyGameState
    local GameState = UE.UGameLobbyFunctionLibrary.GetCurrentLobbyGameState(self)
    if GameState then
        local CurrentExperience = GameState:GetGameplayExperience()
        if CurrentExperience then
            InitMapInfo(self, CurrentExperience)
        end
    end
end

function GameLobbyMainView:PostInitialized()
    local NewVM = InstantiateViewModel(GameLobbyMainVM)
    self:BindViewModel(NewVM, {
        {BindKey = "ReadyButtonSwitcher",   UIKey = "StartGameButtonSwitcher",   DataBinding = DataBinding.SwitcherIndexBinding() },
        {BindKey = "ReadyButtonText",       UIKey = "ReadyButton_Text",          DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "AIPlayerCount",         UIKey = "AICountDisplayText",        DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "MainListSwitcher",      UIKey = "MainListView_Switcher",     DataBinding = DataBinding.SwitcherIndexBinding() },
        {BindKey = "MapPreviewImage",       UIKey = "MapPreviewImage",           DataBinding = DataBinding.TextureAssetBinding(false) },
        {BindKey = "MapNameLabel",          UIKey = "MapName",                   DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "ShowChangeMapButton",   UIKey = "ChangeMap_Button",          DataBinding = DataBinding.WidgetVisibilityBinding(true) },
        {BindKey = "ShowAICountSlider",     UIKey = "AIPlayerCountSliderContainer", DataBinding = DataBinding.WidgetVisibilityBinding(false) },
    })

    ---@type ListViewWrapper 
    self.PlayerList = ListWrapper.New(self, self.PlayerListView)
    self.MapList = ListWrapper.New(self, self.MapListView)

    ---@type LuaEventService
    local LuaEventService = GetGameService(self, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_OnPlayerEnterLobby, self, self.OnPlayerListChanged)
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_OnPlayerLeaveLobby, self, self.OnPlayerListChanged)
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_OnGameReadyChanged, self, self.UpdateGameReadyStatus)
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_OnGameplayExperienceChanged, self, self.OnGameplayExperienceChanged)
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_OnAIPlayerCountChanged, self, self.OnAIPlayerCountChanged)
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_NotifyGameLoading, self, self.NotifyGameLoading)
    end

    -- Change ready button based on net role
    self.bIsServer = UE.UKismetSystemLibrary.IsServer(self)
    self.ViewModel.ReadyButtonSwitcher = self.bIsServer and 0 or 1
    self.ViewModel.ShowAICountSlider = self.bIsServer
    self.ViewModel.ShowChangeMapButton = self.bIsServer

    self.ReadyButton.OnClicked:Add(self, self.OnReadyButtonClicked)
    self.StartGameButton.OnClicked:Add(self, self.OnStartGameButtoClicked)
    self.BackButton.OnClicked:Add(self, self.OnBackButtonClicked)
    self.ChangeMap_Button.OnClicked:Add(self, self.OnChangeMapButtonClicked)
    self.ConfirmMap_Button.OnClicked:Add(self, self.OnConfirmMapButtonClicked)
    self.CancelMap_Button.OnClicked:Add(self, self.OnCancelMapButtonClicked)
    self.AIPlayerCountSlider.OnValueChanged:Add(self, self.OnAIPlayerSliderChanged)
    self.MapListView.BP_OnItemSelectionChanged:Add(self, self.OnMapListSelectChanged)

    --- Init AI count
    ---@type LuaGameInstance
    local GameInstance = UE.ULuaIntegrationFunctionLibrary.GetGameInstance(self)
    if GameInstance then
        self.AIPlayerCountSlider:SetValue(GameInstance.GameAICount)
    end

    -- Update player list and game ready state
    self:OnPlayerListChanged()

    -- Listen local player state changes
    RegisterCallback(self)

    -- Load game experiences
    if self.bIsServer then
        LoadGameExperiences(self)
    else
        DisplayCurrentExperience(self)
    end
end

function GameLobbyMainView:UnInitialize()
    ---@type LuaEventService
    local LuaEventService = GetGameService(self, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_OnPlayerEnterLobby, self, self.OnPlayerListChanged)
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_OnPlayerLeaveLobby, self, self.OnPlayerListChanged)
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_OnGameReadyChanged, self, self.UpdateGameReadyStatus)
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_OnGameplayExperienceChanged, self, self.OnGameplayExperienceChanged)
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_OnAIPlayerCountChanged, self, self.OnAIPlayerCountChanged)
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_NotifyGameLoading, self, self.NotifyGameLoading)
    end

    self.ReadyButton.OnClicked:Remove(self, self.OnReadyButtonClicked)
    self.StartGameButton.OnClicked:Remove(self, self.OnStartGameButtoClicked)
    self.BackButton.OnClicked:Remove(self, self.OnBackButtonClicked)
    self.ChangeMap_Button.OnClicked:Remove(self, self.OnChangeMapButtonClicked)
    self.ConfirmMap_Button.OnClicked:Remove(self, self.OnConfirmMapButtonClicked)
    self.CancelMap_Button.OnClicked:Remove(self, self.OnCancelMapButtonClicked)
    self.AIPlayerCountSlider.OnValueChanged:Remove(self, self.OnAIPlayerSliderChanged)
    self.MapListView.BP_OnItemSelectionChanged:Remove(self, self.OnMapListSelectChanged)

    -- Stop listening local player state changes
    local LocalPlayerState = UE.UGameLobbyFunctionLibrary.GetLocalGameLobbyPlayerState(self)
    if LocalPlayerState then
        LocalPlayerState.OnReadyStatusChanged:Remove(self, self.OnLocalPlayerReadyStatusChanged)
    end

    -- Stop listening local player status changes
    UnRegisterCallback(self)
end

function GameLobbyMainView:OnPlayerListChanged()
    local PlayerStateTable = UE.UGameLobbyFunctionLibrary.GetAllGameLobbyPlayerState(self):ToTable()
    if #PlayerStateTable > 0 then
        self.PlayerList:LoadDataByList(PlayerStateTable)
    else
        self.PlayerList:Clear()
    end

    self:UpdateGameReadyStatus()
end

function GameLobbyMainView:UpdateGameReadyStatus(bReady)
    -- Only update game ready status on server side
    if not self.bIsServer then
        return
    end

    -- Get value from game state if not passed in
    if bReady == nil then
        local GameState = UE.UGameLobbyFunctionLibrary.GetCurrentLobbyGameState()
        bReady = GameState and GameState:GetGameReadyStatus() or false
    end

    self.StartGameButton:SetIsEnabled(bReady)
end

function GameLobbyMainView:OnReadyButtonClicked()
    --- Register
    if not self.LocalPlayerState then
        RegisterCallback(self)
    end

    --- Disable until ready status updated
    self.ReadyButton:SetIsEnabled(false)

    local LocalPlayerState = UE.UGameLobbyFunctionLibrary.GetLocalGameLobbyPlayerState(self)
    local LocalPlayerController = UE.UGameLobbyFunctionLibrary.GetLocalGameLobbyPlayerController(self)
    if LocalPlayerState and LocalPlayerController then
        local bIsReady = LocalPlayerState:GetPlayerReadyStatus()
        LocalPlayerController:ServerMarkPlayerReady(not bIsReady)
    end
end

function GameLobbyMainView:OnLocalPlayerReadyStatusChanged(InPlayerState, bReady)
    self.ReadyButton:SetIsEnabled(true)

    self.ViewModel.ReadyButtonText = GetLocalizedString(LocalizationTable.CommonUI, bReady and "UI_Cancel" or "UI_Ready")
end

function GameLobbyMainView:OnAIPlayerSliderChanged(InValue)
    InValue = math.ceil(InValue)    

    if self.bIsServer then
        local GameState = UE.UGameLobbyFunctionLibrary.GetCurrentLobbyGameState(self)
        if GameState then
            GameState:ServerSetAIPlayerCount(InValue)
        end
    end
end

function GameLobbyMainView:OnAIPlayerCountChanged(InCount)
    self.ViewModel.AIPlayerCount = string.format("%d", InCount)
end

function GameLobbyMainView:OnStartGameButtoClicked()
    if self.SelectedGameExperience then
        ---@type GameplayDataSubsystem
        local GameplayDataSubsystem = UE.UCommonGameplayFunctionLibrary.GetGameplayDataSubsystem(self)
        if GameplayDataSubsystem then
            GameplayDataSubsystem:LoadGameplayExperience(self.SelectedGameExperience)
        end
    end
end

function GameLobbyMainView:OnBackButtonClicked()
    -- ---@type UCommonSessionSubsystem
    -- local CommonSessionSubsystem = UE.UGameLobbyFunctionLibrary.GetCommonSessionSubSystem(self)
    -- if CommonSessionSubsystem then
    --     CommonSessionSubsystem:CleanUpSessions()
    -- end

    -- Dismiss session
    UE.UGameLobbyFunctionLibrary.DismissGameLobby(self)

    -- Back to main menu
    UE.UGameplayStatics.OpenLevel(self, "/Game/DogFightGame/Level/MainMenu/MainMenu")
end

function GameLobbyMainView:OnChangeMapButtonClicked()
    self.ViewModel.MainListSwitcher = 1
end

function GameLobbyMainView:OnConfirmMapButtonClicked()
    if self.PendingGameExperience then
        self.SelectedGameExperience = self.PendingGameExperience

        ---@type AGameLobbyGameState
        local GameState = UE.UGameLobbyFunctionLibrary.GetCurrentLobbyGameState(self)
        if GameState then
            GameState:ServerSetGameplayExperience(self.PendingGameExperience)
        end
    end
    self.ViewModel.MainListSwitcher = 0
end

function GameLobbyMainView:OnGameplayExperienceChanged()
    ---@type AGameLobbyGameState
    local GameState = UE.UGameLobbyFunctionLibrary.GetCurrentLobbyGameState(self)
    if GameState then
        local CurrentExperience = GameState:GetGameplayExperience()
        if CurrentExperience then
            InitMapInfo(self, CurrentExperience)
        end
    end
end

function GameLobbyMainView:OnCancelMapButtonClicked()
    self.ViewModel.MainListSwitcher = 0
end

---@param bSelected boolean
function GameLobbyMainView:OnMapListSelectChanged(Item, bSelected)
    if bSelected then
        self.PendingGameExperience = Item:GetData()
    end
end

function GameLobbyMainView:NotifyGameLoading()
    ---@type GameWidgetService
    local GameWidgetService = GetGameService(self, GameServiceNameDef.GameWidgetService)
    if GameWidgetService then
        GameWidgetService:ShowWidget(GameWidgetNameDef.WidgetGameLoading, true)
    end
end

return GameLobbyMainView