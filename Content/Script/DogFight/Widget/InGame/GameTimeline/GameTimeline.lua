local GameTimeline = UnrealClass("Common.MVVM.ModelBase")
local ViewModelBase = require("Common.MVVM.ViewModelBase")
local DataBinding = require("Common.MVVM.DataBinding")
local GameTimelineVM = require("DogFight.Widget.InGame.GameTimeline.GameTimelineVM")
local ListWrapper = require("Common.ListView.ListViewWrapper")

function GameTimeline:Initialize()
    local NewVM = InstantiateViewModel(GameTimelineVM)
    self:BindViewModel(NewVM, {
        --{BindKey = "TitleMessage",   UIKey = "TitleMessage_Text",   DataBinding = DataBinding.TextContextBinding(), }
    })
end

function GameTimeline:PostInitialized()
    ---@type ListViewWrapper List wrapper for timeline player list.
    self.TimelineViewList = ListWrapper.New(self, self.Timeline_ListView)

    GetGameService(self, GameServiceNameDef.LuaEventService):RegisterListener(UE.ELuaEvent.LuaEvent_GameTimelineChanged, self, self.OnTimelineChanged)
end

function GameTimeline:OnTimelineChanged()
    -- Get current timeline
    local CurTimeline = UE.UCommonGameFlowFunctionLibrary.GetCurrentTimeline(self)

    if CurTimeline:Length() > 0 then
        print("Show timeline: ")
        for i = 1, CurTimeline:Length() do
            print(i .. ": " .. CurTimeline:Get(i))
        end
    end

    local TimelineData = CurTimeline:ToTable()
    self.TimelineViewList:LoadDataByList(TimelineData)
end

return GameTimeline