---@class CommonPropertyDisplayView : BP_Widget_CommonPropertyDisplay_C 
local CommonPropertyDisplayView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local CommonPropertyDisplayVM = require("DogFight.Widget.Common.CommonPropertyDisplay.CommonPropertyDisplayVM")
--local ListWrapper = require("Common.ListView.ListViewWrapper")

function CommonPropertyDisplayView:PostInitialized()
    local NewVM = InstantiateViewModel(CommonPropertyDisplayVM)
    self:BindViewModel(NewVM, {
        {BindKey = "PropertyName",      UIKey = "PropertyName_Text",    DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "PropertyValue",     UIKey = "PropertyValue_Text",   DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "PropertyValue",     UIKey = "EditValue_TextBox",    DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "EditStatus",        UIKey = "Status_Switcher",      DataBinding = DataBinding.SwitcherIndexBinding() },
    })

    ---@type ListViewWrapper 
    --self.MyListWrapper = ListWrapper.New(self, self.ListView)

    self.EditValue_TextBox.OnTextCommitted:Add(self, self.OnPropertyValueCommitted)
end

function CommonPropertyDisplayView:UnInitialize()
    self.EditValue_TextBox.OnTextCommitted:Remove(self, self.OnPropertyValueCommitted)
end

function CommonPropertyDisplayView:OnShow()
end

function CommonPropertyDisplayView:OnListItemObjectSet(InObject)
    self.Data = InObject:GetData()

    if self.Data then
        self.ViewModel.EditStatus = self.Data.bEditable and 1 or 0
        self.ViewModel.PropertyName = self.Data.Name or ""
        self.ViewModel.PropertyValue = self.Data.Value or ""

        -- Record callback
        self.ValueChangeCallback = self.Data.OnChangeCallback

        self.bCheckEmpty = self.Data.bCheckEmpty
        self.CacheValue = self.Data.Value
    end
end

function CommonPropertyDisplayView:OnPropertyValueCommitted(InText, Type)
    -- Skip empty value
    if self.bCheckEmpty then
        if #InText == 0 then
            self.ViewModel.PropertyValue = self.CacheValue
            return
        end
    end
    -- Update new value
    self.CacheValue = InText

    if type(self.ValueChangeCallback) == "function" then
        self.ValueChangeCallback(InText)
    end
end

return CommonPropertyDisplayView