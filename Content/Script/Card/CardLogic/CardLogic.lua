local CardCommandHelper = require "Card.CardCommand.CardCommandHelper"
local CardCommandResultDef = require "Card.CardCommand.CardCommandResultDef"

---@field LogicCommand CardLogicCommand
---@class CardLogic : UCardLogic Card logic is the actual class binding with UCardLogic class and holding all commands.
local CardLogic = Class()

function CardLogic:OnLogicStarted(ScriptPath)
    -- Create and execute card logic command
    self.LogicCommand = CardCommandHelper.CreateCardCommand(ScriptPath)
    if self.LogicCommand then
        self.LogicCommand._CardLogic = self
        self.LogicCommand:SetCallback(self, self.OnCardLogicFinished)
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
    local FinishType = UE.ECardLogicFinishType.Success
    if Result == CardCommandResultDef.Succeed then
        FinishType = UE.ECardLogicFinishType.Success
    elseif Result == CardCommandResultDef.Cancelled then
        FinishType = UE.ECardLogicFinishType.Cancel
    else
        FinishType = UE.ECardLogicFinishType.Failed
    end
    self:MarkLogicFinished(FinishType)
end

return CardLogic