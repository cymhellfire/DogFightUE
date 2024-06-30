---@field bReadonly boolean Is this info panel readonly.
---@class AvatarMiniInfoView : BP_Widget_AvatarMiniInfo_C 
local AvatarMiniInfoView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local AvatarMiniInfoVM = require("DogFight.Widget.Common.AvatarMiniInfo.AvatarMiniInfoVM")
--local ListWrapper = require("Common.ListView.ListViewWrapper")

function AvatarMiniInfoView:PostInitialized()
    local NewVM = InstantiateViewModel(AvatarMiniInfoVM)
    self:BindViewModel(NewVM, {
        {BindKey = "AvatarName",   UIKey = "AvatarName_Text",   DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "ReadonlyState", UIKey = "ReadOnly_Switcher",    DataBinding = DataBinding.SwitcherIndexBinding() },
    })

    ---@type ListViewWrapper 
    --self.MyListWrapper = ListWrapper.New(self, self.ListView)

    -- Load options
    self.AvatarSelect_ComboBox:ClearOptions()
    ---@type AvatarManagerService
    local AvatarManagerService = GetGameService(self, GameServiceNameDef.AvatarManagerService)
    if AvatarManagerService then
        local OptionList = AvatarManagerService:GetAvatarSelectOptionList()
        for _, v in ipairs(OptionList) do
            self.AvatarSelect_ComboBox:AddOption(v)
        end
    end

    self.AvatarSelect_ComboBox.OnSelectionChanged:Add(self, self.OnSelectedOptionChanged)
end

function AvatarMiniInfoView:UnInitialize()
    self.AvatarSelect_ComboBox.OnSelectionChanged:Remove(self, self.OnSelectedOptionChanged)
end

function AvatarMiniInfoView:OnShow()
end

---Initialize with avatar id
---@param InId FGameLobbyPlayerAvatarId
function AvatarMiniInfoView:InitInfo(InId, Readonly, Callback)
    self.ViewModel.ReadonlyState = Readonly and 0 or 1

    if not Readonly then
        self.OnChangeCallback = Callback
    else
        self.OnChangeCallback = nil
    end

    self:SetAvatarId(InId)
end

---Set config id of display avatar
---@param InId FGameLobbyPlayerAvatarId
function AvatarMiniInfoView:SetAvatarId(InId)
    ---@type AvatarManagerService
    local AvatarManagerService = GetGameService(self, GameServiceNameDef.AvatarManagerService)
    if AvatarManagerService then
        if self.ViewModel.ReadonlyState == 1 then
            self.bInit = true
            self.AvatarSelect_ComboBox:SetSelectedIndex(AvatarManagerService:GetOptionIndexByAvatarId(InId))
            self.bInit = nil
        else
            self.ViewModel.AvatarName = AvatarManagerService:GetOptionLocalizationString(InId)
        end
    end
end

function AvatarMiniInfoView:OnSelectedOptionChanged(Item, SelectionType)
    -- Skip if triggered by initializing
    if self.bInit then
        return
    end

    print("Selected avatar ", Item)

    if type(self.OnChangeCallback) == "function" then
        local Index = self.AvatarSelect_ComboBox:GetSelectedIndex() + 1

        ---@type AvatarManagerService
        local AvatarManagerService = GetGameService(self, GameServiceNameDef.AvatarManagerService)
        if AvatarManagerService then
            local AvatarId = AvatarManagerService:GetLobbyPlayerAvatarId(Index)
            self.OnChangeCallback(AvatarId)
        end
    end
end

return AvatarMiniInfoView