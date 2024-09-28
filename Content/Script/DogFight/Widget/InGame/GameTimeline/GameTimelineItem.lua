local GameTimelineItem = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local GameTimelineItemVM = require("DogFight.Widget.InGame.GameTimeline.GameTimelineItemVM")

function GameTimelineItem:PostInitialized()
    local NewVM = InstantiateViewModel(GameTimelineItemVM)
    self:BindViewModel(NewVM, {
        {BindKey = "PlayerName",   UIKey = "PlayerName_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })
end

function GameTimelineItem:OnListItemObjectSet(InObject)
    if InObject and InObject["GetData"] then
        local EntryDesc = InObject:GetData()
        if EntryDesc then
            self.ViewModel.PlayerName = EntryDesc.DescString
        end
    end   
end

return GameTimelineItem