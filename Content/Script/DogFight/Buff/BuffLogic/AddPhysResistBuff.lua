local CharacterModifierTypeDef = require "DogFight.Services.CharacterModifierService.CharacterModifierTypeDef"

---@class AddPhysResistBuff : BuffLogicBase Add physics resistance buff.
---@field _Modifier UCharacterStatusModifier Character status modifier instance.
local AddPhysResistBuff = UnrealClass("DogFight.Buff.BuffLogic.BuffLogicBase")

---@param InCharacter ATopDownStylePlayerCharacter
function AddPhysResistBuff:OnApply(InCharacter)
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
    end
end

---@param InCharacter ATopDownStylePlayerCharacter
function AddPhysResistBuff:OnRemove(InCharacter)
    self.Super.OnRemove(self, InCharacter)

    -- Remove modifier from character
    if InCharacter and InCharacter.DamageReceiverComponent then
        if self._Modifier then
            InCharacter.DamageReceiverComponent:RemoveModifierObject(self._Modifier)
        end
    end
end

function AddPhysResistBuff:tostring()
    return "AddPhysResistBuff"
end

return AddPhysResistBuff