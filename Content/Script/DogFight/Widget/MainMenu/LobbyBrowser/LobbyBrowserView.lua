---@class LobbyBrowserView : BP_Widget_LobbyBrowser_C
---@field GameSearchRequest UCommonSession_SearchSessionRequest
---@field SelectItem GenericListViewItem Selected item
local LobbyBrowserView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local LobbyBrowserVM = require("DogFight.Widget.MainMenu.LobbyBrowser.LobbyBrowserVM")
local ListWrapper = require("Common.ListView.ListViewWrapper")

local GameWidgetNameDef = require("DogFight.Services.GameWidgetService.GameWidgetNameDef")
local GameLuaStateNameDef = require("DogFight.Services.GameStateMachineService.GameLuaStateNameDef")

function LobbyBrowserView:PostInitialized()
    local NewVM = InstantiateViewModel(LobbyBrowserVM)
    self:BindViewModel(NewVM, {
        {BindKey = "LobbyListSwitcher",   UIKey = "LobbyList_Switcher",   DataBinding = DataBinding.SwitcherIndexBinding() },
        {BindKey = "RefreshButtonEnable",   UIKey = "RefreshLobby_Button",  DataBinding = DataBinding.WidgetEnableBinding() },
        {BindKey = "JoinButtonEnable",      UIKey = "JoinLobby_Button",     DataBinding = DataBinding.WidgetEnableBinding() },
    })

    ---@type ListViewWrapper 
    self.LobbyList = ListWrapper.New(self, self.LobbyList_ListView)

    self.Exit_Button.OnClicked:Add(self, self.OnExitButtonClicked)
    self.RefreshLobby_Button.OnClicked:Add(self, self.OnRefreshButtonClicked)
    self.JoinLobby_Button.OnClicked:Add(self, self.OnJoinLobbyButtonClicked)
    self.LobbyList_ListView.BP_OnItemSelectionChanged:Add(self, self.OnLobbyItemSelectionChanged)
end

function LobbyBrowserView:UnInitialize()
    -- Release search request
    if self.GameSearchRequest then
        UnLua.Unref(self.GameSearchRequest)
        self.GameSearchRequest = nil
    end

    self.Exit_Button.OnClicked:Remove(self, self.OnExitButtonClicked)
    self.RefreshLobby_Button.OnClicked:Remove(self, self.OnRefreshButtonClicked)
    self.JoinLobby_Button.OnClicked:Remove(self, self.OnJoinLobbyButtonClicked)
    self.LobbyList_ListView.BP_OnItemSelectionChanged:Remove(self, self.OnLobbyItemSelectionChanged)
end

function LobbyBrowserView:OnShow()
    -- Request lobby list once show
    self:OnRefreshButtonClicked()
end

---@param self LobbyBrowserView
local function RefreshLobbyList(self)
    -- Clear selection first
    self.SelectItem = nil
    self.ViewModel.JoinButtonEnable = false

    -- Disable refresh button until finished
    self.ViewModel.RefreshButtonEnable = false

    ---@type UCommonSessionSubsystem
    local SessionSubsystem = UE.UGameLobbyFunctionLibrary.GetCommonSessionSubSystem(self)
    if SessionSubsystem then
        -- Create search request if not exist
        if self.GameSearchRequest == nil then
            local Request = SessionSubsystem:CreateOnlineSearchSessionRequest()
            Request.OnlineMode = UE.ECommonSessionOnlineMode.LAN
            Request.bUseLobbies = true

            self.GameSearchRequest = Request
            -- Keep the reference
            UnLua.Ref(self.GameSearchRequest)
        end

        -- Register callback
        self.GameSearchRequest.K2_OnSearchFinished:Add(self, self.OnSearchFinished)

        -- Send request
        local LocalPC = UE.ULuaIntegrationFunctionLibrary.GetFirstLocalPlayerController(self)
        SessionSubsystem:FindSessions(LocalPC, self.GameSearchRequest)

        self.ViewModel.LobbyListSwitcher = 0
    end
end

function LobbyBrowserView:OnExitButtonClicked()
    ---@type GameWidgetService
    local WidgetService = GetGameService(self, GameServiceNameDef.GameWidgetService)
    if WidgetService then
        WidgetService:RemoveWidget(GameWidgetNameDef.WidgetLobbyBrowser)
    end
end

function LobbyBrowserView:OnRefreshButtonClicked()
    RefreshLobbyList(self)
end

function LobbyBrowserView:OnSearchFinished(bSuccess, Msg)
    -- Print result
    print("Search game: " .. tostring(bSuccess) .. " Msg: " .. Msg)

    -- Recover refresh button
    self.ViewModel.RefreshButtonEnable = true

    if self.GameSearchRequest then
        self.GameSearchRequest.K2_OnSearchFinished:Remove(self, self.OnSearchFinished)

        -- Load all games into lobby list
        local FoundGames = self.GameSearchRequest.Results:ToTable()        
        self.LobbyList:LoadDataByList(FoundGames)

        self.ViewModel.LobbyListSwitcher = (#FoundGames > 0) and 1 or 2
    end
end

---@param Item GenericListViewItem
---@param bSelected boolean
function LobbyBrowserView:OnLobbyItemSelectionChanged(Item, bSelected)
    if bSelected then
        self.SelectItem = Item

        self.ViewModel.JoinButtonEnable = true
    end
end

function LobbyBrowserView:OnJoinLobbyButtonClicked()
    if self.SelectItem then
        ---@type UCommonSession_SearchResult
        local SearchResult = self.SelectItem:GetData()
        if SearchResult then
            ---@type UCommonSessionSubsystem
            local SessionSubsystem = UE.UGameLobbyFunctionLibrary.GetCommonSessionSubSystem(self)
            if SessionSubsystem then
                SessionSubsystem.K2_OnJoinSessionCompleteEvent:Add(self, self.OnJoinSessionSuccess)

                local LocalPC = UE.ULuaIntegrationFunctionLibrary.GetFirstLocalPlayerController(self)
                SessionSubsystem:JoinSession(LocalPC, SearchResult)
            end
        end

        ---@type GameWidgetService
        local GameWidgetService = GetGameService(self, GameServiceNameDef.GameWidgetService)
        if GameWidgetService then
            GameWidgetService:RemoveWidget(GameWidgetNameDef.WidgetLobbyBrowser)
            GameWidgetService:ShowWidget(GameWidgetNameDef.WidgetJoiningSession, true)
        end
    end
end

function LobbyBrowserView:OnJoinSessionSuccess()
    ---@type UCommonSessionSubsystem
    local SessionSubsystem = UE.UGameLobbyFunctionLibrary.GetCommonSessionSubSystem(self)
    if SessionSubsystem then
        SessionSubsystem.K2_OnJoinSessionCompleteEvent:Remove(self, self.OnJoinSessionSuccess)
    end

    ---@type GameWidgetService
    local GameWidgetService = GetGameService(self, GameServiceNameDef.GameWidgetService)
    if GameWidgetService then
        GameWidgetService:RemoveWidget(GameWidgetNameDef.WidgetJoiningSession)
    end

    ---Let gameplay subsystem listening network failures
    ---@type GameplayDataSubsystem
    local GameplayDataSubsystem = UE.UCommonGameplayFunctionLibrary.GetGameplayDataSubsystem(self)
    if GameplayDataSubsystem then
        GameplayDataSubsystem:StartListenNetworkFailure()
    end

    ---@type GameStateMachineService
    local GameStateMachineService = GetGameService(self, GameServiceNameDef.GameStateMachineService)
    if GameStateMachineService then
        GameStateMachineService:TryEnterState(GameLuaStateNameDef.StateGameLobby, { bWaitingSession = true, })
    end
end

return LobbyBrowserView