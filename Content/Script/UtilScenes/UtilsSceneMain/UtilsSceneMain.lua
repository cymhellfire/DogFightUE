require "UnLua"

---@class UtilsSceneMain : BP_Widget_UtilsSceneMain_C
local UtilsSceneMain = Class("Common.MVVM.ModelBase")
local UtilsSceneMainVM = require("UtilScenes.UtilsSceneMain.UtilsSceneMainVM")
local DataBinding = require("Common.MVVM.DataBinding")
local NumberHelper = require("Common.NumberHelper")

local UtilMenuIndex = {
    UI_GameEffectPreviewMenuTitle = 0,
    UI_ProjectilePreviewMenuTitle = 1,
}

function UtilsSceneMain:PostInitialized()
    local NewVM = InstantiateViewModel(UtilsSceneMainVM)
    self:BindViewModel(NewVM, {
        {BindKey = "MenuTypeSwitcher",      UIKey = "MenuType_Switcher",    DataBinding = DataBinding.SwitcherIndexBinding() },
        {BindKey = "LightIntensityText",    UIKey = "LightIntensity_Text",  DataBinding = DataBinding.TextContextBinding() },
    })

    self.MenuType_ComboBox.OnSelectionChanged:Add(self, self.OnMenuTypeSelected)
    self.LightIntensity_Slider.OnValueChanged:Add(self, self.OnLightIntensityChanged)
end

function UtilsSceneMain:OnMenuTypeSelected(Item)
    -- Check if it's necessary to swtich menu
    local NewIndex = UtilMenuIndex[Item]
    local CurIndex = self.ViewModel.MenuTypeSwitcher
    if NewIndex == CurIndex then
        return
    end

    -- Notify current menu
    local CurMenu = self.MenuType_Switcher:GetActiveWidget()
    if CurMenu.OnSwitchOff then
        CurMenu:OnSwitchOff()
    end

    -- Switch to corresponding menu
    if NewIndex then
        self.ViewModel.MenuTypeSwitcher = NewIndex
    end
end

function UtilsSceneMain:OnLightIntensityChanged(InValue)
    self.ViewModel.LightIntensityText = NumberHelper.format_num(InValue, 2)
    ---@type UtilSceneController
    local PlayerController = UE.UGameplayStatics.GetPlayerController(self, 0)
    if PlayerController then
        PlayerController:SetDirectionalLightIntensity(InValue)
    end
end

return UtilsSceneMain