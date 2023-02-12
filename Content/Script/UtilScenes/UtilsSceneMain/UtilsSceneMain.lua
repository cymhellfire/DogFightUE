require "UnLua"

---@class UtilsSceneMain : BP_Widget_UtilsSceneMain_C
local UtilsSceneMain = Class("Common.MVVM.ModelBase")
local UtilsSceneMainVM = require("UtilScenes.UtilsSceneMain.UtilsSceneMainVM")
local DataBinding = require("Common.MVVM.DataBinding")

local UtilMenuIndex = {
    UI_GameEffectPreviewMenuTitle = 0,
    UI_ProjectilePreviewMenuTitle = 1,
}

function UtilsSceneMain:PostInitialized()
    local NewVM = InstantiateViewModel(UtilsSceneMainVM)
    self:BindViewModel(NewVM, {
        {BindKey = "MenuTypeSwitcher",   UIKey = "MenuType_Switcher",   DataBinding = DataBinding.SwitcherIndexBinding(), }
    })

    self.MenuType_ComboBox.OnSelectionChanged:Add(self, self.OnMenuTypeSelected)
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

return UtilsSceneMain