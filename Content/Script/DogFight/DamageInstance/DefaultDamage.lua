require "UnLua"

local DefaultDamage = Class()

function DefaultDamage:K2_PreApplyToComponent(DamageEvent)
    self:SetDamageType(UE.UExtendedDamageType.StaticClass())
    print("DefaultDamage:PreApplyToComponent from " .. DamageEvent.DamageCauser:GetName())
end

return DefaultDamage
