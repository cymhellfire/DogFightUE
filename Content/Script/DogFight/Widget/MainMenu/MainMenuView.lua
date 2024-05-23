---@class MainMenuView : BP_Widget_MainMenu_C
---@field GameSearchRequest UCommonSession_SearchSessionRequest
local MainMenuView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local MainMenuVM = require("DogFight.Widget.MainMenu.MainMenuVM")
--local ListWrapper = require("Common.ListView.ListViewWrapper")

local GameWidgetNameDef = require("DogFight.Services.GameWidgetService.GameWidgetNameDef")

function MainMenuView:PostInitialized()
    local NewVM = InstantiateViewModel(MainMenuVM)
    self:BindViewModel(NewVM, {
        --{BindKey = "TitleMessage",   UIKey = "TitleMessage_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })

    ---@type ListViewWrapper 
    --self.MyListWrapper = ListWrapper.New(self, self.ListView)

    self.HostGame_Button.OnClicked:Add(self, self.OnHostGameButtonClicked)
    self.SearchGame_Button.OnClicked:Add(self, self.OnSearchGameButtonClicked)
    self.PreparationRoom_Button.OnClicked:Add(self, self.OnPreparationRoomButtonClicked)
    self.ExitGame_Button.OnClicked:Add(self, self.OnExitButtonClicked)
end

function MainMenuView:UnInitialize()

    self.HostGame_Button.OnClicked:Remove(self, self.OnHostGameButtonClicked)
    self.SearchGame_Button.OnClicked:Remove(self, self.OnSearchGameButtonClicked)
    self.PreparationRoom_Button.OnClicked:Remove(self, self.OnPreparationRoomButtonClicked)
    self.ExitGame_Button.OnClicked:Remove(self, self.OnExitButtonClicked)
end

function MainMenuView:OnHostGameButtonClicked()
    -- ---@type UCommonSessionSubsystem
    -- local SessionSubsystem = UE.UGameLobbyFunctionLibrary.GetCommonSessionSubSystem(self)
    -- if SessionSubsystem then
    --     local Request = SessionSubsystem:CreateOnlineHostSessionRequest()
    --     Request.OnlineMode = UE.ECommonSessionOnlineMode.LAN
    --     Request.ModeNameForAdvertisement = "Test Game"

    --     local LocalPC = UE.ULuaIntegrationFunctionLibrary.GetFirstLocalPlayerController(self)
    --     SessionSubsystem:HostSession(LocalPC, Request)
    -- end

    ---@type GameWidgetService
    local WidgetService = GetGameService(self, GameServiceNameDef.GameWidgetService)
    if WidgetService then
        WidgetService:ShowWidget(GameWidgetNameDef.WidgetCreateLobbyDialog, true)
    end
end

function MainMenuView:OnSearchGameButtonClicked()
    -- ---@type UCommonSessionSubsystem
    -- local SessionSubsystem = UE.UGameLobbyFunctionLibrary.GetCommonSessionSubSystem(self)
    -- if SessionSubsystem then
    --     local Request = SessionSubsystem:CreateOnlineSearchSessionRequest()
    --     Request.OnlineMode = UE.ECommonSessionOnlineMode.LAN
    --     Request.bUseLobbies = true
    --     Request.K2_OnSearchFinished:Add(self, self.OnSearchFinished)

    --     self.GameSearchRequest = Request

    --     local LocalPC = UE.ULuaIntegrationFunctionLibrary.GetFirstLocalPlayerController(self)
    --     SessionSubsystem:FindSessions(LocalPC, Request)
    -- end

    ---@type GameWidgetService
    local WidgetService = GetGameService(self, GameServiceNameDef.GameWidgetService)
    if WidgetService then
        WidgetService:ShowWidget(GameWidgetNameDef.WidgetLobbyBrowser, true)
    end
end

function MainMenuView:OnSearchFinished(bSuccess, Msg)
    -- Print result
    print("Search game: " .. tostring(bSuccess) .. " Msg: " .. Msg)

    if self.GameSearchRequest then
        self.GameSearchRequest.K2_OnSearchFinished:Remove(self, self.OnSearchFinished)

        -- print all found games
        local FoundGames = self.GameSearchRequest.Results:ToTable()
        for i = 1, #FoundGames do
            ---@type UCommonSession_SearchResult
            local Game = FoundGames[i]
            local Desc = Game:GetDescription() or "null"

            print(string.format("Game[%d]: %s", i, Desc))
        end
    end
end

function MainMenuView:OnPreparationRoomButtonClicked()
    ---@type GameWidgetService
    local WidgetService = GetGameService(self, GameServiceNameDef.GameWidgetService)
    if WidgetService then
        WidgetService:ShowWidget(GameWidgetNameDef.WidgetPreparationRoom, true)
    end
end

function MainMenuView:OnExitButtonClicked()
    ---@type UCommonSessionSubsystem
    local SessionSubsystem = UE.UGameLobbyFunctionLibrary.GetCommonSessionSubSystem(self)
    if SessionSubsystem then
        SessionSubsystem:CleanUpSessions()
    end
end

return MainMenuView