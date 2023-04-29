local CardCommandHelper = require "Card.CardCommand.CardCommandHelper"
local CardCommandResultDef = require "Card.CardCommand.CardCommandResultDef"

---@field LogicCommand CardLogicCommand
---@class CardLogic : UCardLogic Card logic is the actual class binding with UCardLogic class and holding all commands.
local CardLogic = UnrealClass()

---Load logic script and initialize this logic. (Invoke by UCardLogic::InitLogic)
---@param ScriptPath string Logic script path.
function CardLogic:LoadAndInitLogicScript(ScriptPath)
    -- Instantiate logic script
    self.LogicCommand = CardCommandHelper.CreateCardCommand(ScriptPath)
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