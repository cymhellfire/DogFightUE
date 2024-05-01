local CardActionCommand = require "Card.CardCommand.CardActionCommand"
local CardCommandHelper = require "Card.CardCommand.CardCommandHelper"

---Card action that enqueue an input to weapon.
---@class ActionWeaponInput : CardActionCommand
local ActionWeaponInput = UnrealClass(CardActionCommand)

function ActionWeaponInput:InitWeaponInputCommand(InTarget)
    self._Target = InTarget
end

local function RegisterListener(self, ArsenalComponent)
    ---@type DelegateHelperService
    local DelegateHelperService = GetGameService(self._CardLogic, GameServiceNameDef.DelegateHelperService)
    if DelegateHelperService then
        self.OnWeaponFinishDelegate = DelegateHelperService:BindCallback(ArsenalComponent.OnSlotWeaponFinished, 
            self, self.OnSlotWeaponFinished)
    end
end

local function UnregisterListener(self)
    ---@type DelegateHelperService
    local DelegateHelperService = GetGameService(self._CardLogic, GameServiceNameDef.DelegateHelperService)
    if DelegateHelperService then
        DelegateHelperService:ReleaseDelegateHelper(self.OnWeaponFinishDelegate)
        self.OnWeaponFinishDelegate = nil
    end
end

---Command executing function
function ActionWeaponInput:StartCommand()
    CardActionCommand.StartCommand(self)

    if not self._Target then
        self:FailedWithParams()
        return
    end

    local Target = self._Target[1]
    if Target.Type ~= UE.ECardTargetType.CTT_Actor then
        self:FailedWithParams()
        return
    end

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

    ---@type UArsenalComponent
    local ArsenalComponent = Character:GetArsenalComponent()
    if not ArsenalComponent then
        self:FailedWithParams()
        return
    end

    -- Register callback to ArsenalComponent
    RegisterListener(self, ArsenalComponent)
    self._ArsenalComponent = ArsenalComponent

    local UsedSlot = UE.UCommonGameplayFunctionLibrary.CharacterAttack(Character, Target.Target)
    if UsedSlot == UE.EWeaponSlotType.WST_None then
        print("[ActionWeaponInput] Cannot attack target.")
        UnregisterListener(self)
        self:FailedWithParams()
        return
    end

    -- Record used weapon slot
    self._WeaponSlot = UsedSlot
end

function ActionWeaponInput:OnSlotWeaponFinished(InSlot)
    -- Success when weapon at used slot finished
    if InSlot == self._WeaponSlot then
        UnregisterListener(self)
        self:SuccessWithParams()
    end
end

function ActionWeaponInput:tostring()
    return "ActionWeaponInput"
end

return ActionWeaponInput
