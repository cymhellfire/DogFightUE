---@class CardModifierListView : BP_Widget_CardModifierList_C
local CardModifierListView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local CardModifierListVM = require("DogFight.Widget.InGame.CardModifierList.CardModifierListVM")
local ListWrapper = require("Common.ListView.ListViewWrapper")

function CardModifierListView:PostInitialized()
    local NewVM = InstantiateViewModel(CardModifierListVM)
    self:BindViewModel(NewVM, {
        --{BindKey = "TitleMessage",   UIKey = "TitleMessage_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })

    ---@type ListViewWrapper 
    self.ModifierList = ListWrapper.New(self, self.ModifierList_ListView)
end

---@param InAttrList TArray<UAttributeWrapperObjectBase> Input attribute wrapper array.
---@param OutList table<UAttributeModifierDescObject> List of found modifiers
local function GetAllModifierList(InAttrList, OutList)
    OutList = OutList or {}
    if InAttrList and InAttrList:Length() > 0 then
        for i = 1, InAttrList:Length() do
            ---@type UAttributeWrapperObjectBase
            local Attr = InAttrList:Get(i)
            local ModifierList = Attr and Attr.AppliedModifierDesc or nil
            if ModifierList and ModifierList:Length() > 0 then
                for j = 1, ModifierList:Length() do
                    OutList[#OutList + 1] = ModifierList:Get(j)
                end
            end            
        end
    end
end

---Initialize modifier list with given card description object.
---@param InDescObject UCardDescObject
function CardModifierListView:InitModifierList(InDescObject)
    local AllModifierObject = {}
    ---@type TArray<UAttributeBooleanWrapperObject>
    local BoolAttrList = InDescObject and InDescObject.BooleanWrapperList or nil
    GetAllModifierList(BoolAttrList, AllModifierObject)

    local IntAttrList = InDescObject and InDescObject.IntegerWrapperList or nil
    GetAllModifierList(IntAttrList, AllModifierObject)

    local FloatAttrList = InDescObject and InDescObject.FloatWrapperList or nil
    GetAllModifierList(FloatAttrList, AllModifierObject)

    -- Load all modifier data
    self.ModifierList:LoadDataByList(AllModifierObject)
end

return CardModifierListView