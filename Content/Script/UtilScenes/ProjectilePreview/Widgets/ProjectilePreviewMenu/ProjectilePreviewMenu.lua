require "UnLua"

---@class ProjectilePreviewMenu : BP_Widget_ProjectilePreviewMenu_C
local ProjectilePreviewMenu = Class("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local ProjectilePreviewMenuVM = require("UtilScenes.ProjectilePreview.Widgets.ProjectilePreviewMenu.ProjectilePreviewMenuVM")
local ListWrapper = require("Common.ListView.ListViewWrapper")

function ProjectilePreviewMenu:PostInitialized()
    local NewVM = InstantiateViewModel(ProjectilePreviewMenuVM)
    self:BindViewModel(NewVM, {
        --{BindKey = "TitleMessage",   UIKey = "TitleMessage_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })

    self.ProjectileListWrapper = ListWrapper.New(self, self.ProjectileList_ListView)
    self:InitProjectileList()
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

return ProjectilePreviewMenu