---@class LobbyBrowserView : BP_Widget_LobbyBrowser_C
---@field GameSearchRequest UCommonSession_SearchSessionRequest
---@field SelectItem GenericListViewItem Selected item
local LobbyBrowserView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local LobbyBrowserVM = require("DogFight.Widget.MainMenu.LobbyBrowser.LobbyBrowserVM")
local ListWrapper = require("Common.ListView.ListViewWrapper")

local GameWidgetNameDef = require("DogFight.Services.GameWidgetService.GameWidgetNameDef")

function LobbyBrowserView:PostInitialized()
    local NewVM = InstantiateViewModel(LobbyBrowserVM)
    self:BindViewModel(NewVM, {
        {BindKey = "LobbyListSwitcher",   UIKey = "LobbyList_Switcher",   DataBinding = DataBinding.SwitcherIndexBinding() }
    })

    ---@type ListViewWrapper 
    self.LobbyList = ListWrapper.New(self, self.LobbyList_ListView)

    self.Exit_Button.OnClicked:Add(self, self.OnExitButtonClicked)
    self.RefreshLobby_Button.OnClicked:Add(self, self.OnRefreshButtonClicked)
    self.JoinLobby_Button.OnClicked:Add(self, self.OnJoinLobbyButtonClicked)
    self.LobbyList_ListView.BP_OnItemSelectionChanged:Add(self, self.OnLobbyItemSelectionChanged)

    -- Request lobby list once show
    self:OnRefreshButtonClicked()
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

---@param self LobbyBrowserView
local function RefreshLobbyList(self)
    --- Clear selection first
    self.SelectItem = nil

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
                local LocalPC = UE.ULuaIntegrationFunctionLibrary.GetFirstLocalPlayerController(self)
                SessionSubsystem:JoinSession(LocalPC, SearchResult)
            end
        end
    end
end

return LobbyBrowserView