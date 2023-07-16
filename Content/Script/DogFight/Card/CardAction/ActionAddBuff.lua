local CardActionCommand = require "Card.CardCommand.CardActionCommand"
local CardTargetHelper = require "Card.CardTarget.CardTargetHelper"
local CardCommandHelper = require "Card.CardCommand.CardCommandHelper"

---@field _BuffId number Id of buff to add.
---@field _BuffDuration number The lifespan of the created buff (in round unit).
---@field _TargetList table Target list this command executing with.
---@class ActionAddBuff : CardActionCommand Action that add a buff to given character.
local ActionAddBuff = UnrealClass(CardActionCommand)

---Initialize the buff creating settings.
---@param InTable table Create parameters.
function ActionAddBuff:InitBuffSettings(InTable)
    self._Arguments = self:ConvertArgumentTable(InTable)
    self._BuffId = self._Arguments.BuffId
    self._BuffDuration = self._Arguments.Duration
    self._TargetList = InTable.TargetList
end

---@param self ActionAddBuff
---@param InTarget ATopDownStylePlayerCharacter
local function CreateBuffAndApply(self, InTarget)
    if not InTarget or not self._BuffId then
        return
    end

    ---@type BuffService
    local BuffService = GetGameService(self._CardLogic, GameServiceNameDef.BuffService)
    if BuffService then
        ---@type BuffBase
        local NewBuff = BuffService:CreateBuff(self._BuffId)
        if NewBuff then
            -- Setup the lifespan
            if type(self._BuffDuration) == "number" and type(NewBuff.SetDuration) == "function" then
                NewBuff:SetDuration(self._BuffDuration)
            end
            -- Initialize buff
            if type(self._Arguments) == "table" and type(NewBuff.SetupArgument) == "function" then
                NewBuff:SetupArgument(self._Arguments)
            end
            InTarget:AddBuff(NewBuff)
        end
    end
end

function ActionAddBuff:StartCommand()
    CardActionCommand.StartCommand(self)

    local bFailed = false
    -- Check target count
    if CardTargetHelper.GetTargetCountByType(self._TargetList, UE.ECardTargetType.CTT_Actor) == 0 then
        bFailed = true
    end

    -- Check buff settings
    if not self._BuffId then
        bFailed = true
    end

    if bFailed then
        self:FailedWithParams()
        return
    end

    -- Create new buff for each actor type target
    for _, v in ipairs(self._TargetList) do
        if v.Type and v.Type == UE.ECardTargetType.CTT_Actor then
            CreateBuffAndApply(self, v.Target)
        end
    end

    self:SuccessWithParams()
end

return ActionAddBuff