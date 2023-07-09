---@class NumSliderView : BP_Widget_NumSlider_C
---@field bIsInteger boolean Whether this slider use integer value.
---@field TextFormat string Format string for number display
local NumSliderView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local NumSliderVM = require("Common.NumSlider.NumSliderVM")
--local ListWrapper = require("Common.ListView.ListViewWrapper")

function NumSliderView:PostInitialized()
    local NewVM = InstantiateViewModel(NumSliderVM)
    self:BindViewModel(NewVM, {
        {BindKey = "PlayerNum",         UIKey = "Num_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })

    ---@type ListViewWrapper 
    --self.MyListWrapper = ListWrapper.New(self, self.ListView)

    self.TextFormat = "%d"

    self.Num_Slider.OnValueChanged:Add(self, self.OnSliderValueChanged)
end

---@param self NumSliderView
---@param InValue number
local function SyncNumberDisplay(self, InValue)
    if self.bIsInteger then
        InValue = math.ceil(InValue)
    end
    self.ViewModel.PlayerNum = string.format(self.TextFormat, InValue)
end

function NumSliderView:UnInitialize()
    self.Num_Slider.OnValueChanged:Remove(self, self.OnSliderValueChanged)
end

function NumSliderView:OnSliderValueChanged(InValue)
    SyncNumberDisplay(self, InValue)
end

function NumSliderView:InitSliderMode(InParam)
    if type(InParam) ~= "table" then
        return
    end

    self.bIsInteger = InParam["bIsInteger"] or false
    self.TextFormat = self.bIsInteger and "%d" or "%.2f"
    self.Num_Slider.MouseUsesStep = self.bIsInteger
    self.Num_Slider:SetMaxValue(InParam["Max"] or 1)
    self.Num_Slider:SetMinValue(InParam["Min"] or 0)
    self.Num_Slider:SetStepSize(InParam["Step"] or 0.01)
    self.Num_Slider:SetValue(InParam["Value"] or 0)

    SyncNumberDisplay(self, self.Num_Slider:GetValue())
end

---@param InValue number
function NumSliderView:SetValue(InValue)
    if type(InValue) ~= "number" then
        return
    end

    self.Num_Slider:SetValue(InValue)
end

---@return number Current value
function NumSliderView:GetValue()
    return self.Num_Slider:GetValue()
end

return NumSliderView