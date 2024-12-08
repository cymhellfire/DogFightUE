---@class PokerInfoView : ModelBase 
local PokerInfoView = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local PokerInfoVM = require("DogFight.Widget.Common.PokerInfo.PokerInfoVM")
--local ListWrapper = require("Common.ListView.ListViewWrapper")
local json = require("Common.json")

function PokerInfoView:PostInitialized()
    local NewVM = InstantiateViewModel(PokerInfoVM)
    self:BindViewModel(NewVM, {
        {BindKey = "NumberText",        UIKey = "Num_Text",         DataBinding = DataBinding.TextContextBinding() },
        {BindKey = "SymbolImage",       UIKey = "Icon_Image",       DataBinding = DataBinding.TexturePathBinding(false) },
        {BindKey = "SymbolColor",       UIKey = "Icon_Image",       DataBinding = DataBinding.ColorAndOpacityBinding() },
    })

    ---@type ListViewWrapper 
    --self.MyListWrapper = ListWrapper.New(self, self.ListView)
end

function PokerInfoView:UnInitialize()

end

function PokerInfoView:OnShow()

end

function PokerInfoView:InitInfo(Info)
    if type(Info) ~= "string" then
        return
    end

    -- Decode info string
    local DecodeInfo = json.decode(Info)
    if not DecodeInfo then
        return
    end

    self.ViewModel.NumberText = DecodeInfo.NumberText

    -- Symbol
    local SymbolSettings = DecodeInfo.SymbolSettings
    if SymbolSettings then
        self.ViewModel.SymbolImage = SymbolSettings.IconPath
        local Color
        local ColorTable = SymbolSettings.Color
        if ColorTable then
            Color = UE.FLinearColor(ColorTable.R, ColorTable.G, ColorTable.B, ColorTable.A)
        else
            Color = UE.FLinearColor(0, 0, 0, 1)
        end
        self.ViewModel.SymbolColor = Color
    end
end

return PokerInfoView