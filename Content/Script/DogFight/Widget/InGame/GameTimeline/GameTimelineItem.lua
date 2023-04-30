local GameTimelineItem = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local GameTimelineItemVM = require("DogFight.Widget.InGame.GameTimeline.GameTimelineItemVM")

function GameTimelineItem:Initialize()
    local NewVM = InstantiateViewModel(GameTimelineItemVM)
    self:BindViewModel(NewVM, {
        {BindKey = "PlayerName",   UIKey = "PlayerName_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })


end

function GameTimelineItem:OnListItemObjectSet(InObject)
    if InObject and InObject["GetData"] then
        local PlayerId = InObject:GetData()
        if PlayerId then
            self.ViewModel.PlayerName = "Player " .. PlayerId
        end
    end   
end

return GameTimelineItem