---@class DefaultDamage : UExtendedDamageInstance
local DefaultDamage = UnrealClass()

local function ApplyResistance(BaseValue, Resistance)
    return math.max(BaseValue - Resistance, 0)
end

---@param DamageEvent FExtendedDamageEvent @[out]
function DefaultDamage:K2_PreApplyToComponent(DamageEvent)
    self:SetDamageType(UE.UExtendedDamageType.StaticClass())
    print("DefaultDamage:PreApplyToComponent from " .. DamageEvent.DamageCauser:GetName())

    -- Get physics resistance of target component
    ---@type UDamageReceiverComponent
    local TargetComponent = DamageEvent.ReceiverComponent
    if TargetComponent then
        local bHasValue, PhysResist = TargetComponent:GetAttributeIntegerValue("PhysResist")
        -- Reduce damage value based on resistance
        DamageEvent.DamageValue = ApplyResistance(DamageEvent.DamageValue, bHasValue and PhysResist or 0)
    end
end

return DefaultDamage
