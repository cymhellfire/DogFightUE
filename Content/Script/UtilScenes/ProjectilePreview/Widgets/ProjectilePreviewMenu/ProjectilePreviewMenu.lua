require "UnLua"

---@class ProjectilePreviewMenu : BP_Widget_ProjectilePreviewMenu_C
local ProjectilePreviewMenu = Class("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local ProjectilePreviewMenuVM = require("UtilScenes.ProjectilePreview.Widgets.ProjectilePreviewMenu.ProjectilePreviewMenuVM")
local ListWrapper = require("Common.ListView.ListViewWrapper")
local NumberHelper = require("Common.NumberHelper")

local OptionPanelIndex = {
    UI_ProjectileSpawnMode_Default = 0,
    UI_ProjectileSpawnMode_Drop = 1,
}

function ProjectilePreviewMenu:PostInitialized()
    local NewVM = InstantiateViewModel(ProjectilePreviewMenuVM)
    self:BindViewModel(NewVM, {
        {BindKey = "SpawnOptionSwitcher",   UIKey = "SpawnOption_Switcher",   DataBinding = DataBinding.SwitcherIndexBinding() },
        {BindKey = "DefaultModeSpawnHeight",    UIKey = "DefaultSpawnHeight_Text", DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "DefaultModeTargetHeight",   UIKey = "DefaultTargetHeight_Text", DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "DropModeSpawnHeight",   UIKey = "DropSpawnHeight_Text",     DataBinding = DataBinding.TextContextBinding() },
    })

    self.ProjectileListWrapper = ListWrapper.New(self, self.ProjectileList_ListView)
    self:InitProjectileList()

    self.ProjectileSpawnMode_ComboBox.OnSelectionChanged:Add(self, self.OnSpawnModeChanged)

    self.DefaultSpawnHeight_Slider.OnValueChanged:Add(self, self.OnDefaultSpawnHeightChanged)
    self.DefaultTargetHeight_Slider.OnValueChanged:Add(self, self.OnDefaultTargetHeightChanged)
    self.DropSpawnHeight_Slider.OnValueChanged:Add(self, self.OnDropSpawnHeightChanged)
end

---Initialize projectile list.
function ProjectilePreviewMenu:InitProjectileList()
    ---@type ProjectileService
    local ProjectileService = GetGameService(GameServiceNameDef.ProjectileService)
    if ProjectileService then
        local AllDesc = ProjectileService:GetAllProjectileUtilsDesc()
        local DescTable = AllDesc:ToTable()
        local ItemList = {}
        for _, v in ipairs(DescTable) do
            ItemList[#ItemList + 1] = {
                Id = v.Id,
                Name = v.DisplayName,
            }
        end

        -- Load item list
        self.ProjectileListWrapper:LoadDataByList(ItemList)
    end
end

---Get the player controller in Utility scene
---@return UtilSceneController Player controller in current scene
function ProjectilePreviewMenu:GetUtilController()
    local PlayerController = UE.UGameplayStatics.GetPlayerController(self, 0)
    if PlayerController then
        return PlayerController
    end
end

---Set the projectile id to preview.
---@param InId number Id of projectile want to preview.
function ProjectilePreviewMenu:SetPreviewProjectileId(InId)
    local PlayerController = self:GetUtilController()
    if PlayerController then
        PlayerController:SetProjectileId(InId)
    end
end

function ProjectilePreviewMenu:OnSpawnModeChanged(InMode)
    local PlayerController = self:GetUtilController()
    if PlayerController then
        PlayerController:SetProjectileSpawnMode(InMode)
    end

    -- Switch option panel
    local PanelIndex = OptionPanelIndex[InMode]
    if PanelIndex then
        self.ViewModel.SpawnOptionSwitcher = PanelIndex
    end
end

function ProjectilePreviewMenu:OnDefaultSpawnHeightChanged(InValue)
    self.ViewModel.DefaultModeSpawnHeight = NumberHelper.format_num(InValue, 2)
    local PlayerController = self:GetUtilController()
    if PlayerController then
        PlayerController:SetDefaultModeSpawnHeight(InValue)
    end
end

function ProjectilePreviewMenu:OnDefaultTargetHeightChanged(InValue)
    self.ViewModel.DefaultModeTargetHeight = NumberHelper.format_num(InValue, 2)
    local PlayerController = self:GetUtilController()
    if PlayerController then
        PlayerController:SetDefaultModeTargetHeight(InValue)
    end
end

function ProjectilePreviewMenu:OnDropSpawnHeightChanged(InValue)
    self.ViewModel.DropModeSpawnHeight = NumberHelper.format_num(InValue, 2)
    local PlayerController = self:GetUtilController()
    if PlayerController then
        PlayerController:SetDropModeSpawnHeight(InValue)
    end
end

return ProjectilePreviewMenu