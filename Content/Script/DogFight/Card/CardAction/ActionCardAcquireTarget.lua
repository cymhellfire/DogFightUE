local CardActionCommand = require "Card.CardCommand.CardActionCommand"
local CardTargetHelper = require "Card.CardTarget.CardTargetHelper"
local CardCommandHelper = require "Card.CardCommand.CardCommandHelper"

---@field _Count number Amount of target to acquire.
---@field _Type ECardTargetType Type of target to acquire.
---@field _DelegateHelper DelegateHelper Helper to receive the target acqurie notify.
---@class ActionCardAcquireTarget : CardActionCommand Action to select target for card.
local ActionCardAcquireTarget = UnrealClass(CardActionCommand)

---@param InCount number Target count.
---@param InType ECardTargetType Target type.
function ActionCardAcquireTarget:InitAcquireSettings(InCount, InType)
    self._Count = InCount
    self._Type = InType
end

function ActionCardAcquireTarget:StartCommand()
    CardActionCommand.StartCommand(self)

    ---@type ATopDownStylePlayerController
    local PlayerController = CardCommandHelper.GetOwnerPlayerController(self)
    if PlayerController then  
        ---@type DelegateHelperService
        local DelegateHelperService = GetGameService(self._CardLogic, GameServiceNameDef.DelegateHelperService)
        if DelegateHelperService then
            self._DelegateHelper = DelegateHelperService:BindCallback(PlayerController.OnTargetAcquired, self, self.OnTargetAcquired)
            if self._DelegateHelper then
                -- Start acquire target
                local Setting = UE.FTargetAcquireSettings()
                Setting.TargetCount = self._Count or 1
                Setting.Type = self._Type or UE.ECardTargetType.CTT_Actor
                PlayerController:StartAcquireTargetsWithoutCallback(Setting)
            end
        end
    else
        self:FailedWithParams()
    end
end

---@param bSucceed boolean
---@param TargetInfos TArray<FAcquiredTargetInfo>
function ActionCardAcquireTarget:OnTargetAcquired(bSucceed, TargetInfos)
    -- Release the helper
    if self._DelegateHelper then
        ---@type DelegateHelperService
        local DelegateHelperService = GetGameService(self._CardLogic, GameServiceNameDef.DelegateHelperService)
        if DelegateHelperService then
            DelegateHelperService:ReleaseDelegateHelper(self._DelegateHelper)
            self._DelegateHelper = nil
        end
    end

    -- Skip if not succeed
    if not bSucceed then
        self:FailedWithParams()
        return
    end

    local ConvertTarget = {}
    if TargetInfos:Num() > 0 then
        local Targets = TargetInfos:ToTable()
        for i, v in ipairs(Targets) do
            ConvertTarget[i] = CardTargetHelper.TargetInfoToTable(v)
        end
    end

    self:SuccessWithParams(ConvertTarget)
end

function ActionCardAcquireTarget:tostring()
    return "ActionCardAcquireTarget"
end

return ActionCardAcquireTarget