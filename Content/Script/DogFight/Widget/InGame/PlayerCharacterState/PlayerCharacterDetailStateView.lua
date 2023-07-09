---@class PlayerCharacterDetailStateView : ModelBase 
local PlayerCharacterDetailStateView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local PlayerCharacterDetailStateVM = require("DogFight.Widget.InGame.PlayerCharacterState.PlayerCharacterDetailStateVM")
local ListWrapper = require("Common.ListView.ListViewWrapper")

function PlayerCharacterDetailStateView:PostInitialized()
    local NewVM = InstantiateViewModel(PlayerCharacterDetailStateVM)
    self:BindViewModel(NewVM, {
        {BindKey = "CharacterName",     UIKey = "CharacterName_Text",   DataBinding = DataBinding.TextContextBinding(), },
        --{BindKey = "HealthText",        UIKey = "Health_Text",          DataBinding = DataBinding.TextContextBinding(), },
    })

    ---@type ListViewWrapper 
    self.DRCAttrList = ListWrapper.New(self, self.DRC_AttributeList)
end

---@param InCharacter ATopDownStylePlayerCharacter
---@param MousePos FVector2D
function PlayerCharacterDetailStateView:OnShow(InCharacter, MousePos)
    -- Display info
    self:GatherInfo(InCharacter)

    -- Sync mouse position
    self:SyncPosition(MousePos)
end

---@param InPos FVector2D
function PlayerCharacterDetailStateView:SyncPosition(InPos)
    local ViewportScale = UE.UWidgetLayoutLibrary.GetViewportScale(self)

    UE.UWidgetLayoutLibrary.SlotAsCanvasSlot(self.SyncPositionPanel):SetPosition(UE.FVector2D(InPos.X / ViewportScale, InPos.Y / ViewportScale))
end

---@param InCharacter ATopDownStylePlayerCharacter
function PlayerCharacterDetailStateView:GatherInfo(InCharacter)
    if InCharacter then
        -- Name
        self.ViewModel.CharacterName = InCharacter:GetName()

        -- Attribute
        local AttrItemList = {}
        ---@type UDamageReceiverComponent
        local DRC = InCharacter.DamageReceiverComponent
        if DRC then
            -- Integer attributes
            local IntAttrList = DRC.IntegerWrapperList:ToTable()
            if #IntAttrList > 0 then
                for i = 1, #IntAttrList do
                    ---@type UAttributeIntegerWrapperObject
                    local AttrItem = IntAttrList[i]
                    -- Filter attributes that has origin value and added automatically
                    if not AttrItem:HasFlag(UE.EAttributeFlag.AF_AutoAdd) or not AttrItem:IsBaseValue() then
                        AttrItemList[#AttrItemList + 1] = AttrItem
                    end
                end
            end
        end

        if #AttrItemList > 0 then
            self.DRCAttrList:LoadDataByList(AttrItemList)
        else
            self.DRCAttrList:Clear()
        end
    end
end

return PlayerCharacterDetailStateView