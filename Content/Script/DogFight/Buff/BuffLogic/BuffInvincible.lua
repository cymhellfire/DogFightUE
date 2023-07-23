---@class BuffInvincible : BuffLogicBase 
local BuffInvincible = UnrealClass("DogFight.Buff.BuffLogic.BuffLogicBase")

---Init with self._BuffArgs
function BuffInvincible:OnSetupArgument()

end

---@param InCharacter ATopDownStylePlayerCharacter
function BuffInvincible:OnApply(InCharacter)
    self.Super.OnApply(self, InCharacter)

    -- Apply logic
    if InCharacter then
        UE.UCommonGameplayFunctionLibrary.SetActorInvincible(InCharacter, true, self._Owner)

        ---@type GameEffectService
        local GameEffectService = GetGameService(self._Owner, GameServiceNameDef.GameEffectService)
        if GameEffectService then
            self._Effect = GameEffectService:SpawnEffectAtPos(4, UE.FVector(), UE.FRotator())
            if self._Effect then
                self._Effect:SetTarget(InCharacter)
            end
        end
    end
end

---@param InCharacter ATopDownStylePlayerCharacter
function BuffInvincible:OnRemove(InCharacter)
    self.Super.OnRemove(self, InCharacter)

    -- Remove logic
    if InCharacter then
        UE.UCommonGameplayFunctionLibrary.SetActorInvincible(InCharacter, false, self._Owner);
    end

    -- Remove game effect
    if self._Effect then
        self._Effect:K2_DestroyActor()
        self._Effect = nil
    end
end

function BuffInvincible:tostring()
    return "BuffInvincible"
end

return BuffInvincible