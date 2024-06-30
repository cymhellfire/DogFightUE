---@class GameEffectPreviewMenu Main menu of game effect preview utility scene.
local GameEffectPreviewMenu = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local GameEffectPreviewMenuVM = require("UtilScenes.GameEffectPreview.Widgets.GameEffectPreviewMenu.GameEffectPreviewMenuVM")
local ListWrapper = require("Common.ListView.ListViewWrapper")

function GameEffectPreviewMenu:PostInitialized()
    local NewVM = InstantiateViewModel(GameEffectPreviewMenuVM)
    self:BindViewModel(NewVM, {
        --{BindKey = "TitleMessage",   UIKey = "TitleMessage_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })

    self.EffectListWrapper = ListWrapper.New(self, self.EffectList_ListView)
    self:InitEffectList()
end

function GameEffectPreviewMenu:InitEffectList()
    ---@type GameEffectService
    local GameEffectService = GetGameService(self, GameServiceNameDef.GameEffectService)
    if GameEffectService then
        local AllDesc = GameEffectService:GetAllEffectUtilDesc()
        local DescTable = AllDesc:ToTable()
        local ItemList = {}
        for _, v in ipairs(DescTable) do
            ItemList[#ItemList + 1] = {
                EffectId = v.EffectId,
                EffectName = v.EffectName,
            }
        end

        -- Load data into list
        self.EffectListWrapper:LoadDataByList(ItemList)
    end
end

---Start preview game effect with given id.
---@param InEffectId number Id of game effect to preview.
function GameEffectPreviewMenu:PreviewGameEffect(InEffectId)
    ---@type GameEffectService
    local GameEffectService = GetGameService(self, GameServiceNameDef.GameEffectService)
    if GameEffectService then
        local NewEffect = GameEffectService:SpawnEffectAtPos(InEffectId, UE.FVector(0, 0, 0), UE.FRotator(0, 0, 0))
        if NewEffect then
            NewEffect.OnEffectFinished:Add(self, self.OnPreviewEffectFinished)
            self.CurEffect = NewEffect
        end
    end
end

---Triggered when preview effect finished.
---@param InEffect AGameEffectBase The finished game effect instance.
function GameEffectPreviewMenu:OnPreviewEffectFinished(InEffect)
    -- Stop listening the effect
    InEffect.OnEffectFinished:Remove(self, self.OnPreviewEffectFinished)

    -- No need replay if effect has changed
    if self.CurEffect ~= InEffect then
        return        
    end

    ---@type TimerService
    local TimerService = GetGameService(self, GameServiceNameDef.TimerService)
    if TimerService then
        TimerService:RegisterTimer(self, self.ReplayEffect, 1)
    end
end

function GameEffectPreviewMenu:ReplayEffect()
    if self.CurEffect then
        self:PreviewGameEffect(self.CurEffect.EffectId)
    end
end

---Respond to switcher hide self
function GameEffectPreviewMenu:OnSwitchOff()
    -- Clear record to stop the auto play.
    self.CurEffect = nil
end

return GameEffectPreviewMenu
