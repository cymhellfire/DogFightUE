local CardActionCommand = require "Card.CardCommand.CardActionCommand"
local CardCommandHelper = require "Card.CardCommand.CardCommandHelper"
local CardTargetHelper = require "Card.CardTarget.CardTargetHelper"

---@field _PlayerController ATopDownStylePlayerController Player controller of the moving character.
---@field _DelegateHelper DelegateHelper
---@class ActionMoveToPosition : CardActionCommand
local ActionMoveToPosition = UnrealClass(CardActionCommand)

function ActionMoveToPosition:InitMoveCommand(InTarget)
    self._TargetList = InTarget
end

---Command executing function
function ActionMoveToPosition:StartCommand()
    CardActionCommand.StartCommand(self)

    ---@type ATopDownStylePlayerController
    self._PlayerController = CardCommandHelper.GetOwnerPlayerController(self)
    if not self._PlayerController then
        self:FailedWithParams()
        return
    end

    ---@type ATopDownStylePlayerCharacter
    local Character = self._PlayerController:GetCharacterPawn()
    if not Character then
        self:FailedWithParams()
        return
    end
    
    ---@type DelegateHelperService
    local DelegateHelperService = GetGameService(self._CardLogic, GameServiceNameDef.DelegateHelperService)
    if DelegateHelperService then
        self._DelegateHelper = DelegateHelperService:BindCallback(Character.OnCharacterMoveFinished, self, self.OnCharacterMoveFinished)
    end

    self:StartCharacterMove()
end

function ActionMoveToPosition:StartCharacterMove()
    if #self._TargetList == 0 then
        self:SuccessWithParams()
        return
    end

    local Pos = CardTargetHelper.TargetInfoToPosition(self._TargetList[1])
    if Pos then
        --- Remove the used target
        table.remove(self._TargetList, 1)
        ---@type ATopDownStylePlayerPawn
        local PlayerPawn = self._PlayerController:K2_GetPawn()
        PlayerPawn:ServerMoveCharacter(Pos)
    else
        -- Start with next target info immediately
        self:StartCharacterMove()
    end
end

function ActionMoveToPosition:OnCharacterMoveFinished()
    --- Check with remaining targets
    self:StartCharacterMove()
end

function ActionMoveToPosition:tostring()
    return "ActionMoveToPosition"
end

return ActionMoveToPosition
