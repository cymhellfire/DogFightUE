local CardCommandHelper = require "Card.CardCommand.CardCommandHelper"
local CardCommandResultDef = require "Card.CardCommand.CardCommandResultDef"

---@field SelectTargetCommand CardLogicCommand
---@field LogicCommand CardLogicCommand
---@class CardLogic : UCardLogic Card logic is the actual class binding with UCardLogic class and holding all commands.
local CardLogic = UnrealClass()

local CommandResultToTargetAcquireType = {
    [CardCommandResultDef.Succeed] = UE.ECardTargetAcquireType.Success,
    [CardCommandResultDef.Cancelled] = UE.ECardTargetAcquireType.Cancel,
    [CardCommandResultDef.Failure] = UE.ECardTargetAcquireType.Failed,
}

local CommandResultToCardLogicType = {
    [CardCommandResultDef.Succeed] = UE.ECardLogicFinishType.Success,
    [CardCommandResultDef.Cancelled] = UE.ECardLogicFinishType.Cancel,
    [CardCommandResultDef.Failure] = UE.ECardLogicFinishType.Failed,
}

---Load logic script and initialize this logic. (Invoke by UCardLogic::InitLogic)
---@param CardId number Card config Id.
function CardLogic:LoadAndInitLogicScript(CardId)
    -- Get the card config
    ---@type CardGeneratorService
    local CardGeneratorService = GetGameService(self, GameServiceNameDef.CardGeneratorService)
    if not CardGeneratorService then
        print("[CardLogic] Failed to get card generator service.")
        return
    end
    local CardConfig = CardGeneratorService.Config:GetConfig(CardId)
    if not CardConfig then
        print("[CardLogic] Failed to get card config with id: " .. CardId)
        return
    end
    -- Instantiate select target script
    self.SelectTargetCommand = CardCommandHelper.CreateCardCommand(CardConfig.SelectTargetPath, CardConfig.SelectTargetParam)
    if self.SelectTargetCommand then
        self.SelectTargetCommand._CardLogic = self
        self.SelectTargetCommand:SetCallback(self, self.OnSelectTargetFinished)
    end
    -- Instantiate logic script
    self.LogicCommand = CardCommandHelper.CreateCardCommand(CardConfig.LogicPath, CardConfig.LogicParam)
    if self.LogicCommand then
        self.LogicCommand._CardLogic = self
        self.LogicCommand:SetCallback(self, self.OnCardLogicFinished)
    end
end

---@param DescObject UCardDescObject
function CardLogic:SetupCardDescObject(DescObject)
    -- Use loaded logic script
    if self.LogicCommand then
        -- Check setup function from logic script
        if type(self.LogicCommand.SetupDescObject) == "function" then
            self.LogicCommand:SetupDescObject(DescObject)
        else
            -- Set error name to notify the missing function
            DescObject:SetCardName("Setup Func Missing")
        end
    end
end

---@param DescObject UCardDescObject
function CardLogic:UpdateCardDescObject(DescObject)
    if self.LogicCommand then
        -- Check update function from logic script
        if type(self.LogicCommand.UpdateDescObject) == "function" then
            self.LogicCommand:UpdateDescObject(DescObject)
        else
            -- Set error name to notify the missing function
            DescObject:SetCardName("Update Func Missing")
        end
    end
end

function CardLogic:OnSelectTargetStarted()
    if self.SelectTargetCommand then
        self.SelectTargetCommand:StartCommand()
    end
end

function CardLogic:TickSelectTargetScript(DeltaTime)
    if self.SelectTargetCommand then
        self.SelectTargetCommand:TickCommand(DeltaTime)
    end
end

function CardLogic:OnSelectTargetFinished(Result, TargetInfo)
    print("CardLogic:SelectTargetFinished with result: " .. Result)

    -- Record target info if succeed
    if Result == CardCommandResultDef.Succeed then
        self._TargetInfo = TargetInfo
    end

    local FinishType = CommandResultToTargetAcquireType[Result] or UE.ECardTargetAcquireType.Failed
    self:MarkTargetAcquired(FinishType)
end

function CardLogic:GetTargetInfo()
    return self._TargetInfo
end

function CardLogic:OnLogicStarted()
    -- Execute card logic command if exists
    if self.LogicCommand then
        self.LogicCommand:StartCommand()
    end
end

function CardLogic:TickLogicScript(DeltaTime)
    if self.LogicCommand then
        self.LogicCommand:TickCommand(DeltaTime)
    end
end

function CardLogic:OnCardLogicFinished(Result)
    print("CardLogic:Finish with result: " .. Result)

    -- Convert command result to card logic result
    local FinishType = CommandResultToCardLogicType[Result] or UE.ECardLogicFinishType.Failed
    self:MarkLogicFinished(FinishType)
end

---@param InTarget FAcquiredTargetInfo Target info to push
function CardLogic:PushTargetInfo(InTarget)
    -- Push target info direct into logic command
    if self.LogicCommand then
        self.LogicCommand:AddNativeTargetInfo(InTarget)
    end
end

return CardLogic