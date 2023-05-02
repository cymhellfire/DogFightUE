local CharacterModifierTypeDef = require "DogFight.Services.CharacterModifierService.CharacterModifierTypeDef"

---@class BuffAddPhysResist : BuffLogicBase Add physics resistance buff.
---@field _Modifier UCharacterStatusModifier Character status modifier instance.
---@field _Effect AGameEffectBase Effect instance.
local BuffAddPhysResist = UnrealClass("DogFight.Buff.BuffLogic.BuffLogicBase")

---@param InCharacter ATopDownStylePlayerCharacter
function BuffAddPhysResist:OnApply(InCharacter)
    self.Super.OnApply(self, InCharacter)

    -- Create modifier
    if InCharacter and InCharacter.DamageReceiverComponent then
        ---@type CharacterModifierService
        local CharacterModifierService = GetGameService(self._Owner, GameServiceNameDef.CharacterModifierService)
        if CharacterModifierService then
            self._Modifier = CharacterModifierService:CreateCharacterStatusModifier(CharacterModifierTypeDef.Add10PhysicalResistance)
            if self._Modifier then
                InCharacter.DamageReceiverComponent:AddModifierObject(self._Modifier)
            end
        end

        ---@type GameEffectService
        local GameEffectService = GetGameService(self._Owner, GameServiceNameDef.GameEffectService)
        if GameEffectService then
            self._Effect = GameEffectService:SpawnEffectAtPos(3, UE.FVector(), UE.FRotator())
            if self._Effect then
                self._Effect:SetTarget(InCharacter)
            end
        end
    end
end

---@param InCharacter ATopDownStylePlayerCharacter
function BuffAddPhysResist:OnRemove(InCharacter)
    self.Super.OnRemove(self, InCharacter)

    -- Remove modifier from character
    if InCharacter and InCharacter.DamageReceiverComponent then
        if self._Modifier then
            InCharacter.DamageReceiverComponent:RemoveModifierObject(self._Modifier)
        end
    end

    -- Remove game effect
    if self._Effect then
        self._Effect:K2_DestroyActor()
        self._Effect = nil
    end
end

function BuffAddPhysResist:tostring()
    return "BuffAddPhysResist"
end

return BuffAddPhysResist