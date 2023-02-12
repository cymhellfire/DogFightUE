require "UnLua"

---@class ProjectilePreviewMenuItem : BP_Widget_ProjectilePreviewMenuItem_C
local ProjectilePreviewMenuItem = Class("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local ProjectilePreviewMenuItemVM = require("UtilScenes.ProjectilePreview.Widgets.ProjectilePreviewMenuItem.ProjectilePreviewMenuItemVM")

function ProjectilePreviewMenuItem:PostInitialized()
    local NewVM = InstantiateViewModel(ProjectilePreviewMenuItemVM)
    self:BindViewModel(NewVM, {
        {BindKey = "ProjectileName",    UIKey = "ProjectileName_Text",      DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "SelectState",       UIKey = "SelectState_Switcher",     DataBinding = DataBinding.SwitcherIndexBinding() },
    })

    
end

function ProjectilePreviewMenuItem:OnListItemObjectSet(InObject)
    self.DataObject = InObject
    self.Data = InObject:GetData()
    if self.Data then
        self.ProjectileId = self.Data.Id
        self.ViewModel.ProjectileName = "[" .. self.ProjectileId .. "] " .. self.Data.Name
    end
end

function ProjectilePreviewMenuItem:BP_OnItemSelectionChanged(bIsSelected)
    self.ViewModel.SelectState = bIsSelected and 1 or 0

    -- Play effect if selected
    if bIsSelected then
        self:SelectProjectile()
    end
end

function ProjectilePreviewMenuItem:SelectProjectile()
    ---@type ListViewWrapper
    local ParentWrapper = self.DataObject.ParentWrapper
    if ParentWrapper then
        ---@type ProjectilePreviewMenu
        local PreviewMenu = ParentWrapper.ParentView
        if PreviewMenu then
            PreviewMenu:SetPreviewProjectileId(self.ProjectileId)
        end
    end
end

return ProjectilePreviewMenuItem
