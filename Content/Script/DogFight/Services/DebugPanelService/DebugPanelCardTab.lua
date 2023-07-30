---@param self DebugPanelService
local function LoadCardList(self)
    local CardList = {}
    ---@type CardGeneratorService
    local CardGeneratorService = GetGameService(self, GameServiceNameDef.CardGeneratorService)
    if CardGeneratorService then
        local ConfigNum = CardGeneratorService.Config:GetConfigNum()
        for i = 0, ConfigNum + 1 do
            local CardConfig = CardGeneratorService.Config:GetConfig(i)
            if CardConfig then
                CardList[#CardList + 1] = {
                    Id = i,
                    Name = CardConfig.Name,
                }
            end
        end
    end

    self.CardListCache = CardList
end

local function FilterCardList(CardList, Filter)
    local Result = {}

    -- Ignore case
    local lowerFilter = string.lower(Filter)
    for _, v in ipairs(CardList) do
        if v and v.Name then
            local Start, End = string.find(string.lower(v.Name), lowerFilter)
            if Start and End then
                Result[#Result + 1] = v
            end
        end
    end

    return Result
end

---@param self DebugPanelService
---@param CardId number
local function GiveCard(self, CardId)
    local LocalPC = UE.UCommonGameFlowFunctionLibrary.GetLocalPlayerController(self)
    if LocalPC then
        ---@type CardGameService
        local CardGameService = GetGameService(self, GameServiceNameDef.CardGameService)
        if CardGameService then
            local NewCard
            -- Create new card by name
            if CardId then
                NewCard = CardGameService:CreateCard(CardId, LocalPC)
            end

            -- Dispatch card to player
            local PlayerId = UE.UCommonGameFlowFunctionLibrary.GetLocalPlayerId(self)
            UE.UCommonGameplayFunctionLibrary.DispatchCardToPlayer(self, PlayerId, NewCard)
        end
    end
end

---@param self DebugPanelService
local function DebugPanelCardTab(self)
    -- Load the card list first
    LoadCardList(self)

    self:SetDrawingTab("CardTab")
    self:AddText("Search Card:")
    self:KeepSameLine(20)
    self:AddExecuteText("Search", {self, function(self, Param)
        if type(Param) == "string" then
            print("CardFilter set to: " .. Param)
            self.CardFilter = Param
            -- Rebuild panel after filter changed
            self:RebuildPanel()
        else
            print("OnExecClick")
        end
    end})

    local FilteredCardList = (self.CardFilter and self.CardFilter ~= "") and FilterCardList(self.CardListCache, self.CardFilter)
        or self.CardListCache

    -- Display card list
    if #FilteredCardList > 0 then
        local EntryList = UE.TArray(UE.FDebugPanelComboBoxEntryInfo)
        for i, v in ipairs(FilteredCardList) do
            local NewEntry = UE.FDebugPanelComboBoxEntryInfo()
            NewEntry.Index = i
            NewEntry.DisplayName = v.Name
            EntryList:Add(NewEntry)
        end

        self:AddExecuteComboBox("Get Card", EntryList, {self, function(self, Index)
            local SelectCard = FilteredCardList[Index]
            if SelectCard then
                print("Get card: " .. SelectCard.Name)
                GiveCard(self, SelectCard.Id)
            end 
        end}, UE.FVector2D(200, 200))
    end
end

return DebugPanelCardTab