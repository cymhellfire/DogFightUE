---@class GameLoadingView : ModelBase 
local GameLoadingView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local GameLoadingVM = require("DogFight.Widget.Loading.GameLoading.GameLoadingVM")
--local ListWrapper = require("Common.ListView.ListViewWrapper")

function GameLoadingView:PostInitialized()
    local NewVM = InstantiateViewModel(GameLoadingVM)
    self:BindViewModel(NewVM, {
        {BindKey = "MapTitle",      UIKey = "MapTitle_Text",    DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "MapPreviewImage",   UIKey = "MapPreview_Image", DataBinding = DataBinding.TextureAssetBinding(false) },
    })

    ---@type ListViewWrapper 
    --self.MyListWrapper = ListWrapper.New(self, self.ListView)
end

function GameLoadingView:UnInitialize()

end

function GameLoadingView:OnShow()
    -- Get current gameplay experience
    ---@type AGameLobbyGameState
    local GameState = UE.UGameLobbyFunctionLibrary.GetCurrentLobbyGameState(self)
    if GameState then
        local GameplayExperience = GameState:GetGameplayExperience()
        if GameplayExperience then
            self.ViewModel.MapTitle = GameplayExperience.Title
            self.ViewModel.MapPreviewImage = GameplayExperience.MapPreview
        end
    end
end

return GameLoadingView