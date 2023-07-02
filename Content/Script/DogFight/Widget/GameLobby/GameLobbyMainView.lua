---@class GameLobbyMainView : BP_Widget_GameLobby_C
---@field LocalPlayerState AGameLobbyPlayerState Local player state this UI listen to.
---@field bIsServer boolean Whether current client is hosting server.
local GameLobbyMainView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local GameLobbyMainVM = require("DogFight.Widget.GameLobby.GameLobbyMainVM")
local ListWrapper = require("Common.ListView.ListViewWrapper")

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

function GameLobbyMainView:PostInitialized()
    local NewVM = InstantiateViewModel(GameLobbyMainVM)
    self:BindViewModel(NewVM, {
        {BindKey = "ReadyButtonSwitcher",   UIKey = "StartGameButtonSwitcher",   DataBinding = DataBinding.SwitcherIndexBinding() },
        {BindKey = "ReadyButtonText",       UIKey = "ReadyButton_Text",          DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "AIPlayerCount",         UIKey = "AICountDisplayText",        DataBinding = DataBinding.TextContextBinding() },
    })

    ---@type ListViewWrapper 
    self.PlayerList = ListWrapper.New(self, self.PlayerListView)

    ---@type LuaEventService
    local LuaEventService = GetGameService(self, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_OnPlayerEnterLobby, self, self.OnPlayerListChanged)
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_OnPlayerLeaveLobby, self, self.OnPlayerListChanged)
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_OnGameReadyChanged, self, self.UpdateGameReadyStatus)
        LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_OnAIPlayerCountChanged, self, self.OnAIPlayerCountChanged)
    end

    -- Change ready button based on net role
    self.bIsServer = UE.UKismetSystemLibrary.IsServer(self)
    self.ViewModel.ReadyButtonSwitcher = self.bIsServer and 0 or 1
    self.AIPlayerCountSliderContainer:SetVisibility(self.bIsServer and UE.ESlateVisibility.SelfHitTestInvisible or UE.ESlateVisibility.Collapsed)

    self.ReadyButton.OnClicked:Add(self, self.OnReadyButtonClicked)
    self.StartGameButton.OnClicked:Add(self, self.OnStartGameButtoClicked)
    self.BackButton.OnClicked:Add(self, self.OnBackButtonClicked)
    self.AIPlayerCountSlider.OnValueChanged:Add(self, self.OnAIPlayerSliderChanged)

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
end

function GameLobbyMainView:UnInitialize()
    ---@type LuaEventService
    local LuaEventService = GetGameService(self, GameServiceNameDef.LuaEventService)
    if LuaEventService then
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_OnPlayerEnterLobby, self, self.OnPlayerListChanged)
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_OnPlayerLeaveLobby, self, self.OnPlayerListChanged)
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_OnGameReadyChanged, self, self.UpdateGameReadyStatus)
        LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_OnAIPlayerCountChanged, self, self.OnAIPlayerCountChanged)
    end

    self.ReadyButton.OnClicked:Remove(self, self.OnReadyButtonClicked)
    self.StartGameButton.OnClicked:Remove(self, self.OnStartGameButtoClicked)
    self.BackButton.OnClicked:Remove(self, self.OnBackButtonClicked)
    self.AIPlayerCountSlider.OnValueChanged:Remove(self, self.OnAIPlayerSliderChanged)

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

return GameLobbyMainView