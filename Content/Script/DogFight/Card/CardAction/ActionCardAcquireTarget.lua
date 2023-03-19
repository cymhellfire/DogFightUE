local CardActionCommand = require "Card.CardCommand.CardActionCommand"

---@field _DelegateHelper DelegateHelper Helper to receive the target acqurie notify.
---@class ActionCardAcquireTarget : CardActionCommand Action to select target for card.
local ActionCardAcquireTarget = Class(CardActionCommand)

function ActionCardAcquireTarget:StartCommand()
    CardActionCommand.StartCommand(self)

    local PlayerId = -1
    if self._CardLogic then
        local OwnerCard = self._CardLogic:GetOwnerCard()
        if OwnerCard then
            PlayerId = OwnerCard:GetOwnerPlayerId()
        end
    end

    ---@type ATopDownStylePlayerController
    local PlayerController = PlayerId ~= -1 and UE.UCommonGameplayFunctionLibrary.GetPlayerControllerById(PlayerId) or nil
    if PlayerController then  
        ---@type DelegateHelperService
        local DelegateHelperService = GetGameService(GameServiceNameDef.DelegateHelperService)
        if DelegateHelperService then
            self._DelegateHelper = DelegateHelperService:BindCallback(PlayerController.OnTargetAcquired, self, self.OnTargetAcquired)
            if self._DelegateHelper then
                -- Start acquire target
                local Setting = UE.FTargetAcquireSettings()
                Setting.TargetCount = 1
                Setting.Type = UE.ECardTargetType.CTT_Actor
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
        local DelegateHelperService = GetGameService(GameServiceNameDef.DelegateHelperService)
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

    if TargetInfos:Num() > 0 then
        local Targets = TargetInfos:ToTable()
        for i, v in ipairs(Targets) do
            if v.Type == UE.ECardTargetType.CTT_Actor then
                print("Target acquired: [" .. i .. "] " .. v.ActorPtr:GetName())
            end
        end
    end

    self:SuccessWithParams()
end

function ActionCardAcquireTarget:tostring()
    return "ActionCardAcquireTarget"
end

return ActionCardAcquireTarget