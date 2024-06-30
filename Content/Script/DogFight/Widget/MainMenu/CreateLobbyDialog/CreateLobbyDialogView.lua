---@class CreateLobbyDialogView : BP_Widget_CreateLobbyDialog_C
---@field DefaultLobbyName string
---@field PlayerNum_NumSlider NumSliderView
local CreateLobbyDialogView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local CreateLobbyDialogVM = require("DogFight.Widget.MainMenu.CreateLobbyDialog.CreateLobbyDialogVM")
local GameLuaStateNameDef = require("DogFight.Services.GameStateMachineService.GameLuaStateNameDef")
--local ListWrapper = require("Common.ListView.ListViewWrapper")

function CreateLobbyDialogView:PostInitialized()
    local NewVM = InstantiateViewModel(CreateLobbyDialogVM)
    self:BindViewModel(NewVM, {
        --{BindKey = "TitleMessage",   UIKey = "TitleMessage_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })

    ---@type ListViewWrapper 
    --self.MyListWrapper = ListWrapper.New(self, self.ListView)

    self.DefaultLobbyName = "New Lobby"

    self.PlayerNum_NumSlider:InitSliderMode({
        bIsInteger = true,
        Max = 4,
        Min = 1,
        Step = 1,
        Value = 2,
    })

    self.Confirm_Button.OnClicked:Add(self, self.OnConfirmButtonClicked)
    self.LobbyName_InputBox:SetText(self.DefaultLobbyName)
end

function CreateLobbyDialogView:UnInitialize()
    self.Confirm_Button.OnClicked:Remove(self, self.OnConfirmButtonClicked)
end

function CreateLobbyDialogView:OnConfirmButtonClicked()
    ---@type UCommonSessionSubsystem
    local SessionSubsystem = UE.UGameLobbyFunctionLibrary.GetCommonSessionSubSystem(self)
    if SessionSubsystem then
        -- Find and use the first GameLobby experience
        ---@type UGameplayExperience
        local GameLobbyExperience
        local GameplayDataSubsystem = UE.UCommonGameplayFunctionLibrary.GetGameplayDataSubsystem(self)
        if GameplayDataSubsystem then
            local ExperienceList = GameplayDataSubsystem:GetGameplayExperiencesByType(UE.EGameplayExperienceType.GameLobby):ToTable()
            if #ExperienceList > 0 then
                GameLobbyExperience = ExperienceList[1]
            end
        end

        if not GameLobbyExperience then
            error("Game experience with type GameLobby is not found.")
            return
        end

        SessionSubsystem.K2_OnCreateSessionCompleteEvent:Add(self, self.OnSessionCreated)

        local Request = SessionSubsystem:CreateOnlineHostSessionRequest()
        Request.OnlineMode = UE.ECommonSessionOnlineMode.LAN
        Request.MaxPlayerCount = self.PlayerNum_NumSlider:GetValue()
        Request.MapID = GameLobbyExperience.MapId

        local LocalPC = UE.ULuaIntegrationFunctionLibrary.GetFirstLocalPlayerController(self)
        SessionSubsystem:HostSession(LocalPC, Request)

        ---Switch game state
        ---@type GameStateMachineService
        local GameStateMachineService = GetGameService(self, GameServiceNameDef.GameStateMachineService)
        if GameStateMachineService then
            GameStateMachineService:TryEnterState(GameLuaStateNameDef.StateGameLobby, { bWaitingSession = true, })
        end
    end
end

function CreateLobbyDialogView:OnSessionCreated()
    ---@type UCommonSessionSubsystem
    local SessionSubsystem = UE.UGameLobbyFunctionLibrary.GetCommonSessionSubSystem(self)
    if SessionSubsystem then
        SessionSubsystem.K2_OnCreateSessionCompleteEvent:Remove(self, self.OnSessionCreated)

        SessionSubsystem:UpdateSessionSettings("LobbyName", self.LobbyName_InputBox:GetText() or self.DefaultLobbyName)

        ---Let gameplay subsystem listening network failures
        ---@type GameplayDataSubsystem
        local GameplayDataSubsystem = UE.UCommonGameplayFunctionLibrary.GetGameplayDataSubsystem(self)
        if GameplayDataSubsystem then
            GameplayDataSubsystem:StartListenNetworkFailure()
        end
    end
end

return CreateLobbyDialogView