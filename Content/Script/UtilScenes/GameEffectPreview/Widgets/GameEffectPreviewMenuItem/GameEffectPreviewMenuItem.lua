require "UnLua"

local GameEffectPreviewMenuItem = Class("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local GameEffectPreviewMenuItemVM = require("UtilScenes.GameEffectPreview.Widgets.GameEffectPreviewMenuItem.GameEffectPreviewMenuItemVM")

function GameEffectPreviewMenuItem:PostInitialized()
    local NewVM = InstantiateViewModel(GameEffectPreviewMenuItemVM)
    self:BindViewModel(NewVM, {
        {BindKey = "EffectName",    UIKey = "EffectName_Text",      DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "SelectState",   UIKey = "SelectState_Switcher", DataBinding = DataBinding.SwitcherIndexBinding() },
    })

    
end

function GameEffectPreviewMenuItem:OnListItemObjectSet(InObject)
    self.DataObject = InObject
    self.Data = InObject:GetData()
    if self.Data then
        self.EffectId = self.Data.EffectId
        self.ViewModel.EffectName = "[" .. self.EffectId .. "] " .. self.Data.EffectName
    end
end

function GameEffectPreviewMenuItem:BP_OnItemSelectionChanged(bIsSelected)
    self.ViewModel.SelectState = bIsSelected and 1 or 0

    -- Play effect if selected
    if bIsSelected then
        self:PreviewEffect()
    end
end

function GameEffectPreviewMenuItem:PreviewEffect()
    ---@type ListViewWrapper
    local ParentWrapper = self.DataObject.ParentWrapper
    if ParentWrapper then
        ---@type GameEffectPreviewMenu
        local PreviewMenu = ParentWrapper.ParentView
        if PreviewMenu then
            PreviewMenu:PreviewGameEffect(self.EffectId)
        end
    end
end

return GameEffectPreviewMenuItem
